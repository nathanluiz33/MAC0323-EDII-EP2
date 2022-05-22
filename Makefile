CC=g++-11
CFLAGS=-Wall -g
.PHONY: clean
# ***************************************************************

c = g++-11 -Wall -std=c++17 -lm $< -o $*
mtime = gtime -f '%C %Us %MKB'

%: %.cpp
	$c

generate_tests:
	$(CC) ./gen_tests/gen_texto.cpp -o ./gen_tests/gen_texto
	$(CC) ./gen_tests/gen_texto_ordenado.cpp -o ./gen_tests/gen_texto_ordenado
	$(CC) ./gen_tests/gen_texto_pode_repetidas.cpp -o ./gen_tests/gen_texto_pode_repetidas
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do ./gen_tests/gen_texto $$j < ./Textos/in.texto$$i > ./Inputs/in.EP_$$i-$$j; done; done
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do ./gen_tests/gen_texto_ordenado $$j < ./Textos/in.texto$$i > ./Inputs/in.EP_$$i-$$j-ordenado; done; done
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do ./gen_tests/gen_texto_pode_repetidas $$j < ./Textos/in.texto$$i > ./Inputs/in.EP_$$i-$$j-repetidas; done; done

run_tests:
	$(CC) EP2.cpp -o EP2 

	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do echo "\n== in.EP_$$i-$$j-repetidas ==" && $(mtime) ./EP2 $$j < ./Inputs/in.EP_$$i-$$j-repetidas > ./Outputs/out.EP_$$i-$$j-repetidas; done; done
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do echo "\n== in.EP_$$i-$$j ==" && $(mtime) ./EP2 $$j < ./Inputs/in.EP_$$i-$$j > ./Outputs/out.EP_$$i-$$j; done; done
	@for i in 1 2 3 4 5 6 7; do for j in VO ABB ARN TR A23; do echo "\n== in.EP_$$i-$$j-ordenado ==" && $(mtime) ./EP2 $$j < ./Inputs/in.EP_$$i-$$j-ordenado > ./Outputs/out.EP_$$i-$$j-ordenado; done; done

clean:
	-rm -f *.txt *.in* gen