/** dans un fichier struct.h */
typedef struct s_cmd_handler {
      char * cmd;
      void * (handler*)(char ** );
}                   t_cmd_handler;

typedef struct s_parser {
     char cmd[4];
     char parameters[3][64];
}                             t_parser;

/** dans un fichier carte_X.c */
static void * ra_handler(char ** cmd) {
      // TODO
}

t_cmd_handler handlers[MAX_CMD] = {
           {"Ra", ra_handler},
           [...]
  };


/**
   *    dans un fichier main.c
    */

extern t_cmd_handler handlers[];

static void process(t_parser * parser) {
       for (int i = 0 ; i < MAX_CMD ; i++) {
                 t_cmd_handler handler = handlers[i];
                 if (strcmp(handler.cmd, parser->cmd) == 0) {
                            handler.handler(parser->parameters);
                            return 1;
                 }
      }
      return 0;
}

void trycmd(t_parser * parser) {
      if (process(parser) == 0) {
            write(CARTE_ENFANT, parser, sizeof(t_parser));
      }
}

/**
   * Sur la 1ere carte seulement : transforme la ligne brute en une structure 't_parser',
   * puis appel 'trycmd'
   */
void read(const char * line) {
# ifdef MASTER
      t_parser parser; // TODO : remplir le parser
      trycmd(parser);
# else
    t_parser * parser = (t_parser *) line;
    // OU 
    t_parser parser;
    read(CARTE_PARENTE, &parser, sizeof(t_parser));
# endif
}
