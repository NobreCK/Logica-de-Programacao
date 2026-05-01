from random import randint

def jogo(jogada):
    
    n_user = int(input('Digite um numero de 0 a 5: '))
    n_pc = randint(0, 5)
    total = n_user =+ n_pc
    
    resultado = "PAR" if total % 2 == 0 else "IMPAR"
    
    print(f"Voce jogou {n_user} e o computador jogou {n_pc}, resultado {resultado}")
    
    if (resultado == "PAR" and jogada == "P") or (resultado == "IMPAR" and jogada == "I"):
        print("Voce venceu")    
        return True
    else:
        print('Voce perdeu')
        return False
    
vitorias = 0
    
while True:
    user = str(input('Par ou Impar? [P/I] ')).upper()[0]
    
    if user not in ['P', 'I']:
        print("Escolha invalida ")
        continue
    
    venceu = jogo(user)
    
    if venceu:
        vitorias += 1
        
    else:
        print(f"voce conseguiu {vitorias} vitorias consegutivas")
        break
