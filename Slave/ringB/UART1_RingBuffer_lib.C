//---------------------------------------------------------------------------------------
// ID_1.1    UART0_RingBuffer_lib.C -----------------------------------------------------
//---------------------------------------------------------------------------------------
// ID_1.2  Source originale des codes de buffer circulaire: 
//         Microcontroller Programming III	MP7-46 - 
//         Université d'Adelaide 
//
// ID_1.3 Adapté par F.JOLY - CPE Lyon
// ID_1.4 DATE: 08 Mars 2016 
// ID_1.5 Version 1.0 
// 
// ID_1.6 Objectifs ---------------------------------------------------------------------
// Gestion d'une UART en émission-réception par interruption et buffer circulaire
// 
// ID_1.7 Dépendances matérielles "cible" ----------------------------------------------- 
// Processeur cible: C8051F02x
// Périphérique UART0

// ID_1.8 Dépendances matérielles "extérieures" -----------------------------------------
// 

// ID_1.9 Dépendances de type communications/protocolaires ------------------------------
// La configuration de la communication UART est du type asynchrone - 8bits - 1 stop bit
// Pas de parité
//
// ID_1.10 Fonctionnement du code -------------------------------------------------------
// Le code contient:
// - les fonctions de configuration de l'UART0 et de son timer associé. 
// - Une fonction de création du buffer circulaire (initialisation de structures)
// - Les fonctions de remplissage du buffer de transmission et les fonctions de vidage 
//   du buffer de réception.
//  - La fonction d'interruption UART0 chargée d'émettre sur la liaison série le contenu 
//    du buffer de transmission et de stocker dans le buffer de réception les données 
//    reçues sur la liaison série.

// ID_1.11 Choix technologiques divers --------------------------------------------------
// Utilisation de L'UART0 et du Timer 1 comme source d'horloge de l'UART0.
// Pour fonctionner ce code a besoin des macros Define SYSCLK et BAUDRATE

// ID_1.12 Tests réalisés ---------------------------------------------------------------
// Validation sur plateforme 8051F020TB avec processeur 8051F020
// Vitesse de transmission: 115200 Baud
// Fréquence quartz: 22,1184 MHz
//
// ID_1.13 Chaine de compilation --------------------------------------------------------
// KEIL C51 6.03 / KEIL EVAL C51
//
// ID_1.14 Documentation de référence ---------------------------------------------------
// Datasheet 8051F020/1/2/3  Preliminary Rev. 1.4 12/03 
//
//ID_1.15 Commentaires sur les variables globales et les constantes ---------------------
// La taille des buffers de réception et de transmission est modifiable avec la 
// macro MAX_BUFLEN  

//*************************************************************************************************
#include <c8051F020.h>                    // SFR declarations
#include <stdio.h>
#include <intrins.h>
#include <string.h>
#ifndef CFG_Globale
   #define CFG_Globale
   #include <CFG_Globale.h>
#endif


//*************************************************************************************************
// Paramétresd modifiables
//*************************************************************************************************
#define       MAX_BUFLEN 32 // Taille des buffers de données

//*************************************************************************************************
// DEFINITION DES MACROS DE GESTION DE BUFFER CIRCULAIRE
//*************************************************************************************************

// Structure de gestion de buffer circulaire
	//rb_start: pointeur sur l'adresse de début du buffer 
	// rb_end: pointeur sur l'adresse de fin du buffer	
	// rb_in: pointeur sur la donnée à lire
	// rb_out: pointeur sur la case à écrire
		
#define RB_CREATE(rb, type) \
   struct { \
     type *rb_start; \	   
     type *rb_end; \	   
     type *rb_in; \
	   type *rb_out; \		
	  } rb

//Initialisation de la structure de pointeurs 
// rb: adresse de la structure
// start: adresse du premier élément du buffer 
// number: nombre d'éléments du buffer - 1	(le "-1" n'est à mon avis pas nécessaire)
#define RB_INIT(rb, start, number) \
         ( (rb)->rb_in = (rb)->rb_out= (rb)->rb_start= start, \
           (rb)->rb_end = &(rb)->rb_start[number] )

//Cette macro rend le buffer circulaire. Quand on atteint la fin, on retourne au début
#define RB_SLOT(rb, slot) \
         ( (slot)==(rb)->rb_end? (rb)->rb_start: (slot) )

// Test: Buffer vide? 
#define RB_EMPTY(rb) ( (rb)->rb_in==(rb)->rb_out )

// Test: Buffer plein?
#define RB_FULL(rb)  ( RB_SLOT(rb, (rb)->rb_in+1)==(rb)->rb_out )

// Incrementation du pointeur dur la caseàécrire
#define RB_PUSHADVANCE(rb) ( (rb)->rb_in= RB_SLOT((rb), (rb)->rb_in+1) )

// Incrémentation du pointeur sur la caseàlire
#define RB_POPADVANCE(rb)  ( (rb)->rb_out= RB_SLOT((rb), (rb)->rb_out+1) )

// Pointeur pour stocker une valeur dans le buffer
#define RB_PUSHSLOT(rb) ( (rb)->rb_in )

// pointeur pour lire une valeur dans le buffer
#define RB_POPSLOT(rb)  ( (rb)->rb_out )


//*************************************************************************************************


/* Transmission and Reception Data Buffers */
static char  xdata outbuf[MAX_BUFLEN];     /* memory for transmission ring buffer #1 (TXD) */
static char  xdata inbuf [MAX_BUFLEN];     /* memory for reception ring buffer #2 (RXD) */
static  bit   TXactive = 0;             /* transmission status flag (off) */

/* define out (transmission)  and in (reception)  ring buffer control structures */
static RB_CREATE(out,unsigned char xdata);            /* static struct { ... } out; */
static RB_CREATE(in, unsigned char xdata);            /* static struct { ... } in; */

//**************************************************************************************************
//**************************************************************************************************
void UART1_ISR(void) interrupt 20 {
 
//	static unsigned int cp_tx = 0;
//  static unsigned int cp_rx = 0;
	
  if ((SCON1 & 0x02) == 0x02) // On peut envoyer une nouvelle donnée sur la liaison série
  { 
  	if(!RB_EMPTY(&out)) {
       SBUF1 = *RB_POPSLOT(&out);      /* start transmission of next byte */
       RB_POPADVANCE(&out);            /* remove the sent byte from buffer */
//			 cp_tx++;
  	}
  	else TXactive = 0;                 /* TX finished, interface inactive */
	SCON1 &= 0xFD;
  }
  else // RI0 à 1 - Donc une donnée a été reçue
  {
		if(!RB_FULL(&in)) {                   // si le buffer est plein, la donnée reçue est perdue
     	*RB_PUSHSLOT(&in) = SBUF1;        /* store new data in the buffer */
		  RB_PUSHADVANCE(&in);               /* next write location */
//		  cp_rx++;
	 }
   SCON1 &= 0xFE;
  }
}
// **************************************************************************************************
// init_Serial_Buffer: Initialisation des structures de gestion des buffers transmission et reception
// *************************************************************************************************
//**************************************************************************************************
void init_Serial_Buffer1(void) {

                /* set up RX ring buffer */
RB_INIT(&out, outbuf, MAX_BUFLEN-1);           /* set up TX ring buffer */
    RB_INIT(&in, inbuf,MAX_BUFLEN-1); 
}
// **************************************************************************************************
// SerOutchar: envoi d'un caractére dans le buffer de transmission de la liaison série
// *************************************************************************************************
unsigned char serOutchar1(char c) {

  if(!RB_FULL(&out))  // si le buffer n'est pas plein, on place l'octet dans le buffer
  {                 
  	*RB_PUSHSLOT(&out) = c;               /* store data in the buffer */
  	RB_PUSHADVANCE(&out);                 /* adjust write position */

  	if(!TXactive) {
		TXactive = 1;                      /* indicate ongoing transmission */
 	  SCON1 |= 0x02;//   Placer le bit TI à 1 pour provoquer le déclenchement de l'interruption
  	}
	return 0;  // opération correctement réalisée 
  }
   else return 1; // opération échouée - Typiquement Buffer plein
}
// ************************************************************************************************
//  serInchar: 	lecture d'un caractére dans le buffer de réception de la liaison série
//  Fonction adaptée pour la réception de codes ASCII - La réception de la valeur binaire 0
//  n'est pas possible (conflit avec le code 0 retourné si il n'y a pas de caractére reçu)
// ************************************************************************************************
char serInchar1(void) {
char c;

  if (!RB_EMPTY(&in))
  {                 /* wait for data */

  	c = *RB_POPSLOT(&in);                 /* get character off the buffer */
 	  RB_POPADVANCE(&in);                   /* adjust read position */
  	return c;
  }
  else return 0;
}
// ************************************************************************************************
//  serInchar_Bin: 	lecture d'un caractére dans le buffer de réception de la liaison série
//  Fonction adaptée pour la réception de codes Binaires - Cette fonction retourne un entier. 
//  L'octet de poids faible correspond au caractére reçu, l'octet de poids fort, s'il est nul indique 
//  qu'aucun caractére n'a été reçu.
//  
// ************************************************************************************************
unsigned int serInchar_Bin1(void) {
char c;
unsigned int return_code = 0;
	 
  if (!RB_EMPTY(&in))
  {                
    // un caractére au moins est dans le buffer de réception
  	c = *RB_POPSLOT(&in);                 /* get character off the buffer */
 	  RB_POPADVANCE(&in);                   /* adjust read position */
  	return 0xFF00+c;
  }
	// pas de caractére dans le buffer de réception.
  else return return_code;
}
// *************************************************************************************************
// serOutstring:  Envoi d'une chaine de caractére dans le buffer de transmission
// ************************************************************************************************
unsigned char serOutstring1(char *buf) {
unsigned char len,code_err=0;

  for(len = 0; len < strlen(buf); len++)
     code_err +=serOutchar1(buf[len]);
  return code_err;
}
//*************************************************************************************************
//  CONFIGURATION BAS NIVEAU de L'UART0
//*************************************************************************************************

//*****************************************************************************
#define Preload_Timer0 (SYSCLK/(BAUDRATE*16))
#if Preload_Timer0 > 255 
#error "Valeur Preload Timer0 HORS SPECIFICATIONS"
#endif 
//*****************************************************************************	 
//cfg_Clock_UART
//	Utilisation du Timer 1
//*****************************************************************************	 
void cfg_Clock_UART1(void)
{
  CKCON |= 0x10;      // T1M: Timer 1 use the system clock.
  TMOD |= 0x20;       //  Timer1 CLK = system clock
	TMOD &= 0x2f;			  // Timer1 configuré en timer 8 bit avec auto-reload	
	TF1 = 0;				  // Flag Timer effacé

	TH1 = -(Preload_Timer0);
	ET1 = 0;				   // Interruption Timer 1 dévalidée
	TR1 = 1;				   // Timer1 démarré
}
 
//*****************************************************************************	 
//CFG_uart0_mode1
//
//*****************************************************************************	 
void cfg_UART1_mode1(void)
{
		T4CON &= 0xCF;     // Source clock Timer 1
		PCON  |= 0x10; //SMOD1: UART1 Baud Rate Doubler Disabled.
		PCON &= 0xF7;  // SSTAT1=0
		SCON1 = 0x70;   // Mode 1 - Check Stop bit - Reception validée
		SCON1 |= 0x02;        // Transmission: octet transmis (prét à recevoir un char pour transmettre			
    EIE2 |= 0x40;        // interruption UART0 autorisée	
}

