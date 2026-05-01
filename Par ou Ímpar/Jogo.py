from random import randint

def validador(variavel, arg1, arg2):
    
    if variavel in[arg1, arg2]:
        return True
    
    print("Escolha Invalida")
    return False
 
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
        
        while True:
            escolha = str(input("Quer continuar jogando? [S/N]")).upper()[0]
            if validador(escolha, "S", "N")
                break
                
        if escolha == "N":
            print(f"Finalizado, vitorias totais {vitorias}")
            break   
        
    else:
        print(f"voce conseguiu {vitorias} vitorias consegutivas")
        break
