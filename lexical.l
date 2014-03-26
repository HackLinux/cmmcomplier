%{
	/*copy to head of lexical.c*/

	int chars = 0;
	int words = 0;
	int lines = 0;
%}


digit [0-9]
letter_ [_a-zA-Z]

%%

/*INT*/

/*FLOAT*/

/*ID*/

%%
	/*copy to tail of lexical.c*/

int main(int argc, char** argv){

	if( argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}

	while(yylex() != 0);

	printf("Scanner finished!");

	return 0;
}
