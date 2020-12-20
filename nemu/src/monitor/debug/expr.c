#include <isa.h>
#include <assert.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-",  '-'  },        //sub
  {"/",   '/' },		//div
  {"\\*",  '*'},		//mul
  {"\\(",   '(' }, 
  {"\\)",   ')' },
  {"[[:digit:]]", TK_NUM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[14]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static void my_strcpy( char des[], const char *src, int len ) {
	
	assert( len <= 32 );
	for( int i=0; i<len; i++ ) {
		des[i] = *(src+i);
	}

	des[len] = '\0';
}
static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
	  case TK_NOTYPE : break;
	  case '+' : tokens[nr_token++].type = '+'; break;
	  case '-' : tokens[nr_token++].type = '-'; break; 
	  case '*' : tokens[nr_token++].type = '*'; break;
	  case '/' : tokens[nr_token++].type = '/'; break;
	  case '(' : tokens[nr_token++].type = '('; break;
	  case ')' : tokens[nr_token++].type = ')'; break; 
	  case TK_EQ :tokens[nr_token++].type =  TK_EQ; break;
	  case TK_NUM :tokens[nr_token].type = TK_NUM, my_strcpy( tokens[nr_token++].str, substr_start, substr_len ); break;

          default: assert(0);
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static bool check_parentheses( int p, int q ) {

	int i, tag = 0;
	if( tokens[p].type != '(' || tokens[q].type != ')')
		return false;
	for( i=p; i<=q; i++ ) {
		if( tokens[i].type == '(' )
			tag++;
		else if( tokens[i].type == ')' )
			--tag; 	
		if( tag == 0 && i<q )
			return false;	
	}
	return tag == 0;
}
static int pir( int type ) {

	switch( type ) {
		case '+':
		case '-':  return 0;
		case '*':
		case '/':  return 1;
		default : printf("Invalid op"); return -1;				
	}
	return 0;
}
static int dominant_operator( int p, int q ) {

	int i, dom = p, left_n = 0;
	int pr = 100;
	for( i=p; i<=q; i++ ) { 
		if( tokens[i].type == '(' ) {
			left_n += 1;
			while( i <= q ) { 
				i++;
				if( tokens[i].type == '(' )
					left_n++;
				else if( tokens[i].type == ')' )
					left_n--;
				if( !left_n ) 
					break; 
		} 
		if( i >  q )
			break;
		} else if( tokens[i].type == TK_NUM )
			continue;
		else if( pir( tokens[i].type ) <= pr )  {
			pr = pir(tokens[i].type);
			dom = i;
		}
	}
	return dom;
}
static int eval( int p, int q , bool *invalid ) {

	int num;
	if( p > q ) {
		*invalid = false;
		return 0;
	} else if( p== q ) {
		sscanf( tokens[p].str, "%d", &num );
		return num;
	}	
	else {
		if(check_parentheses( p, q ))
				
			return eval( p+1, q-1, invalid);	
		else {
			int op = dominant_operator( p, q );
			int val1 = eval( p, op-1, invalid);
			int val2 = eval( op+1, q, invalid);
			switch( tokens[op].type ) {

				case '+' :  return val1 + val2;
				case '-' :  return val1 - val2;
				case '*' :  return val1 * val2;
				case '/' :  return val1 / val2;
				default  :  assert(0);	
			}
	
		}
	}
	return 0;
}
word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
    	
   bool invalid = true;
   int res = eval( 0, nr_token-1, &invalid );		
   if( invalid  ) 
	return (word_t)res;
   else 
	success = false;
				 	 
  return 0;
}
