CC=g++-11
CFLAGS=-Wall -g
.PHONY: clean
# ***************************************************************

c = g++-11 -Wall -std=c++17 -lm $< -o $*
mtime = gtime -f '%C %Us %MKB'

%: %.cpp
	$c

# t%: %.cpp
# 	$c -O2
# 	@for i in $*.in*; do echo "\n== $$i ==" && $(mtime) ./$* < $$i; done

mopinha: mopinha.cpp
	$(CC) $^ -o $@ 

all:
	$(CC) Vetor_Ordenado.cpp -o Vetor_Ordenado 
	$(CC) ABB.cpp -o ABB 
	$(CC) TR.cpp -o TR 
	$(CC) ARN.cpp -o ARN
	$(CC) A23.cpp -o A23
	
	echo "-------------------\nA23";
	@for i in in*; do echo "\n== $$i ==" && $(mtime) ./A23 < $$i; done
	echo "-------------------\nTR";
	@for i in in*; do echo "\n== $$i ==" && $(mtime) ./TR < $$i; done
	echo "-------------------\nARN";
	@for i in in*; do echo "\n== $$i ==" && $(mtime) ./ARN < $$i; done
	echo "-------------------\nABB";
	# @for i in in*; do echo "\n== $$i ==" && $(mtime) ./ABB < $$i; done
	echo "\n== 1 ==" && $(mtime) ./ABB < in1;
	echo "\n== 2 ==" && $(mtime) ./ABB < in2;
	echo "\n== 3 ==" && $(mtime) ./ABB < in3;
	echo "\n== 4 ==" && $(mtime) ./ABB < in4;
	echo "\n== ordenado1 ==" && $(mtime) ./ABB < in.ordenado1;
	echo "\n== ordenado2 ==" && $(mtime) ./ABB < in.ordenado2;
	echo "-------------------\nVetor_Ordenado"
	# @for i in in*; do echo "\n== $$i ==" && $(mtime) ./Vetor_Ordenado < $$i; done
	echo "\n== 1 ==" && $(mtime) ./Vetor_Ordenado < in1;
	echo "\n== 2 ==" && $(mtime) ./Vetor_Ordenado < in2;
	echo "\n== ordenado1 ==" && $(mtime) ./Vetor_Ordenado < in.ordenado1;
	echo "\n== ordenado2 ==" && $(mtime) ./Vetor_Ordenado < in.ordenado2;
	echo "\n== ordenado3 ==" && $(mtime) ./Vetor_Ordenado < in.ordenado3;
	echo "\n== ordenado4 ==" && $(mtime) ./Vetor_Ordenado < in.ordenado4;

generate_test:
	$(CC) gen_texto.cpp -o gen_texto
	$(CC) gen_texto_ordenado.cpp -o gen_texto_ordenado
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do ./gen_texto $$j < ./Textos/in.texto$$i > ./Inputs/in.EP_$$i-$$j; done; done
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do ./gen_texto_ordenado $$j < ./Textos/in.texto$$i > ./Inputs/in.EP_$$i-$$j-ordenado; done; done
	

run_tests:
	$(CC) EP2.cpp -o EP2 

	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do echo "\n== in.EP_$$i-$$j ==" && $(mtime) ./EP2 $$j < ./Inputs/in.EP_$$i-$$j > ./Outputs/out.EP_$$i-$$j; done; done
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do echo "\n== in.EP_$$i-$$j-ordenado ==" && $(mtime) ./EP2 $$j < ./Inputs/in.EP_$$i-$$j-ordenado > ./Outputs/out.EP_$$i-$$j-ordenado; done; done

clean:
	-rm -f *.txt *.in* gen