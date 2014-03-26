digit [0-9]

letter_ [_a-zA-Z
%%
%%

int main(int argc, char** argv){

	if( argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror("Cannot open file %s.\n", argv[1]);
			return 1;
		}
	}

	while(yylex() != 0);
	return 0;
}
