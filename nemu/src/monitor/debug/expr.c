#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_NEG, TK_DEREF, TK_REG, TK_NEQ, TK_HEX 

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
	{"[[:digit:]]+", TK_NUM},
	{"-",  '-'},
	{"\\+", '+' },
	{"\\*", '*' },
	{"/"  , '/' },
	{"\\(", '(' },
	{"\\)", ')' },
	{"[0-9a-fA-F]", TK_HEX },
	{"\\$(([\\$rsgt]([0-9a-zA-Z])+)|(pc))", TK_REG},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

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
					case TK_NOTYPE:  break;
					case TK_HEX   :   
					case TK_REG   :  
					case TK_NUM   : my_strcpy(tokens[nr_token].str, substr_start, substr_len); 
          default:  tokens[nr_token++].type = rules[i].token_type;
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
		case TK_EQ :
		case TK_NEQ: return 0;
		case '+':
		case '-':  return 1;
		case TK_NEG: return 2;
		case '*':
		case '/':  return 3;
		case TK_DEREF : return 4;
		default : printf("Invalid op"); assert(0);				
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
		} else if( tokens[i].type == TK_NUM || tokens[i].type == TK_HEX || tokens[i].type == TK_REG )
			continue;
		else if( pir( tokens[i].type ) <= pr )  {
			pr = pir(tokens[i].type);
			dom = i;
		}
	}
	return dom;
}
static word_t eval( int p, int q , bool *invalid ) {

	word_t  num;
	if( p > q ) {
		*invalid = false;
		return 0;
	} else if( p== q ) { 
		if( tokens[p].type == TK_NUM ) {
			sscanf( tokens[p].str, "%d", &num );
			return num;
		} else if( tokens[p].type == TK_HEX ) {
			sscanf( tokens[p].str, "%x", &num );
			return num;	
		} else if( tokens[p].type == TK_REG ) {
			bool success = true;		
			num = isa_reg_str2val( tokens[p].str, &success);
			if( success )
				return num;
			else {
				printf("no match reg\n");
				return 0;
			}			 
		}
	}	
	else {
		if(check_parentheses( p, q ))
				
			return eval( p+1, q-1, invalid);	
		else {
				
			
			int op = dominant_operator( p, q );
			if( tokens[op].type == TK_NEG ) {
				int val = eval( op+1, q, invalid );
				return -val;
			} 
			if( tokens[op].type == TK_DEREF ) {
				extern word_t paddr_read( paddr_t, int );	
				int val = eval( op+1, q ,invalid );
				return paddr_read( val, 4 );
			}	
				
			int val1 = eval( p, op-1, invalid);
			int val2 = eval( op+1, q, invalid);
			switch( tokens[op].type ) {
				case TK_EQ: return val1 == val2;
				case TK_NEQ: return val1 != val2;
				case '+' :  return val1 + val2;
				case '-' :  return val1 - val2;
				case '*' :  return val1 * val2;
				case '/' :  if( val2 == 0 ) { printf("div 0 error\n"); assert(0); } return val1 / val2;		
				default  :  assert(0);	
			}
	
		}
	}
	return 0;
}
static bool is_unary_op( int i ) {

	return  tokens[i].type != TK_NUM && tokens[i].type != TK_HEX && tokens[i].type != ')' ;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */

	
  /* TODO: Insert codes to evaluate the expression. */
	for( int i=0; i<nr_token; i++ ) {	
		if( i == 0 || is_unary_op( i-1 ) ) {
			if( tokens[i].type == '-' )
				tokens[i].type = TK_NEG;
		 //add more op
		 	if( tokens[i].type == '*' )
				tokens[i].type = TK_DEREF;
		}			
  }
	bool invalid = true;
  word_t res = eval( 0, nr_token-1, &invalid );		
  if( invalid  ) 
		return res;
  else 
		success = false;
  return 0;
}
