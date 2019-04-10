# -*- coding: utf-8 -*-
"""
Created on Tue Apr 09 09:32:26 2019

@author: leopa
"""

"""
Ce programme sert à détecter le nombre de bips dans un signal audio format wave,
leur fréquence moyenne,leur durée moyenne, et la durée moyenne des silences
entre les bips.
Ce programme ne marche que pour les bips à moins de 2000Hz (modifiable cependant
dans la boucle while)
Le programme supporte les signaux bruités et non sinusoïdaux.

Executer en ligne de commande sous la forme:
python detec_freq.py fichier.wav

Le fichier detec_freq.py doit être placé dans le même répertoire que le fichier
audio, et ce répertoire doit contenir un autre répertoire nommé logs pour que
l'écriture fichier des données fonctionne.
"""


# Read in a WAV and find the freq's
import pyaudio
import wave
import numpy as np
import datetime
import sys


chunk = 1024 # nombre d'échantillons par chunk
samplerate = 44100 # fréquence d'échantillonnage du signal audio

nom_fich = sys.argv[1] # nom du fichier .wav à fournir en argument lors de l'execution

# open up a wave
wf = wave.open(nom_fich, 'rb')
swidth = wf.getsampwidth()
RATE = wf.getframerate()
# use a Blackman window
window = np.blackman(chunk)
# open stream
p = pyaudio.PyAudio()
stream = p.open(format =
                p.get_format_from_width(wf.getsampwidth()),
                channels = wf.getnchannels(),
                rate = RATE,
                output = True)


# liste de présence de signal par chunk (1 pour présence d'un signal, 0 sinon)
signal = []
# liste des fréquences détectées à chaque chunk
frequences = []

# read some data
data = wf.readframes(chunk)
# play stream and find the frequency of each chunk
while len(data) == chunk*swidth:
    # write data out to the audio stream
    stream.write(data)
    # unpack the data and times by the hamming window
    indata = np.array(wave.struct.unpack("%dh"%(len(data)/swidth),\
                                         data))*window
    
    # Take the fft and square each value
    fftData=abs(np.fft.rfft(indata))**2
    # find the maximum
    which = fftData[1:].argmax() + 1
    # use quadratic interpolation around the max
    if which != len(fftData)-1:
        y0,y1,y2 = np.log(fftData[which-1:which+2:])
        x1 = (y2 - y0) * .5 / (2 * y1 - y2 - y0)
        # find the frequency and output it
        thefreq = (which+x1)*RATE/chunk
        # ajout de la fréquence détectée à la liste
        frequences.append(thefreq)
        # Si la fréquence est inférieure à 2000Hz, on considère que c'est un bip
        # Sinon on considère que c'est du bruit
        if thefreq < 2000:
            # présence d'un bip dans le chunk
            signal.append(1)
        else:
            # absence de bip dans le chunk
            signal.append(0)
    else:
        thefreq = which*RATE/chunk
        # même principe
        frequences.append(thefreq)
        if thefreq < 2000:
            signal.append(1)
        else:
            signal.append(0)
    # read some more data
    data = wf.readframes(chunk)
if data:
    stream.write(data)
stream.close()
p.terminate()

# débruitage :  si un bip a été détecté de façon isolée (1 entouré de deux 0 dans signal)
#               alors on le passe à 0
for i in range(len(signal)):
    if (i<>0) & (i<>len(signal)-1):
        if (signal[i] == 1) & (signal[i-1]==0) & (signal[i+1]==0):
            signal[i] = 0

# ajout d'un zéro à la fin (permet des détecter un bip unique)
signal.append(0)
frequences.append(0)

# compte le nombre de chunks dans le même état
compteur_tps = 0
# liste contenant le nombre de chunk pour chaque bip
liste_bip=[]
# liste contenant le nombre de chunk pour chaque silence
liste_sil=[]
# mémoire du chunk précédent
xprec=signal[0]

for i in signal:
    if i == xprec:
        # incrémentation du compteur de chunks si on est dans le même état
        compteur_tps+=1
    else:
        #sinon ajout du compteur dans la bonne liste
        if i == 0:
            liste_bip.append(compteur_tps)
        else:
            liste_sil.append(compteur_tps)
        # remise à zéro du compteur
        compteur_tps=0
    # mise en mémoire de l'état du chunk
    xprec = i
    
# calcul de la durée moyenne des bips
duree_bip = np.mean(liste_bip)*chunk/samplerate
# calcul de la durée moyenne des silences
duree_sil = np.mean(liste_sil)*chunk/samplerate

# somme des fréquences où un bip a été détecté
sum_freq = 0
nb_freq = 0
for i in range(len(signal)):
    if signal[i]==1:
        sum_freq+=frequences[i]
        nb_freq+=1

# calcul de la fréquence moyenne
freq_moy = sum_freq/nb_freq

# ouverture ou création du fichier log des résultats en mode append
f = open('logs/logfreqfile.txt','a')

# affichage en console et écriture sur le fichier log des résultats
if len(liste_bip)>1:
    print "Il y a %d bips a %d Hz d'environ %d ms espaces d'environ %d ms" % (len(liste_bip), freq_moy, duree_bip*1000, duree_sil*1000)
    f.write("test à la date\t"+ str(datetime.datetime.now())+ "\t"+ nom_fich + "\n"+ str(len(liste_bip))+"\t"+ str(int(freq_moy))+ "\t"+ str(int(duree_bip*1000))+ "\t"+ str(int(duree_sil*1000))+ "\n\n")
else:
    if len(liste_bip)==1:
        print "Il y a un unique bip de %d ms a %d Hz" % (duree_bip*1000, freq_moy)
        f.write("test à la date\t"+ str(datetime.datetime.now())+ "\t"+ nom_fich + "\n"+ str(len(liste_bip))+ "\t"+ str(int(freq_moy))+ "\t"+ str(int(duree_bip*1000))+ "\n\n")
    else:
        print "ERREUR: Aucun bip detecte"
        f.write("test à la date\t"+ str(datetime.datetime.now())+ "\t"+ nom_fich + "\n"+ "ERREUR DETEC\n\n")

#fermeture du fichier
f.close()
