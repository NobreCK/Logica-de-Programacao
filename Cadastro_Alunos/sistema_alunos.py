dados = []
temp = []

def add_dados(n, nota1, nota2):
    temp.append(n)
    temp.append(nota1)
    temp.append(nota2)
    dados.append(temp[:])
    temp.clear() 

def ver(numero):
    return numero <= 10
    
def buscador(nome, excluir, nota1, nota2):
    for i in range(0, len(dados)):
        
        if nome in dados[i][0]:
            if excluir:
                del dados[i]
            if nota1:
                del dados[i][1]
            if nota2:
                del dados[i][2]
            return i
        
while True:       
    try:
        opcoes = int(input("cadastrar, consultar ou excluir ou sair: [1, 2, 3, 4] "))

    except ValueError:
        print("Digite algo valido ")
        continue
        
    match opcoes:
        case 1:
            try:
                quantidade = int(input("quantos alunos voce deseja cadastrar? "))
            except ValueError:
                print("Digite algo valido ")
                continue
            
            for i in range(0, quantidade):
                n = input("nome do aluno: ").lower().strip()
                
                while True:
                    
                    nota1 = float(input("nota 1: "))
                
                    if ver(nota1):
                        break
                    print("valor invalido, digite entre 1 a 10")
                
                while True:
                    nota2 = float(input("nota 2: "))
                    
                    if ver(nota2):
                        break
                    print("valor invalido, digite entre 1 a 10")
                    
                add_dados(n, nota1, nota2)
        case 2:
            if not dados:
                print("nenhum aluno cadastrado")
            else:
                print(dados)
                print()
                nome = input("qual aluno voce deseja consultar? ").strip().lower()
                
                for i in range(0, len(dados)):
               
                    if nome in dados[i][0]:
                   
                        if len(dados[i]) > 2:
                            print(f"aluno: {dados[i][0]} nota 1: {dados[i][1]} nota 2: {dados[i][2]}")
                            print()
                  
                        elif len(dados[i]) > 1:
                            print(f"aluno: {dados[i][0]} nota 1: {dados[i][1]} nota 2: não cadastrada")
                            print()
                   
                        else:
                            print(f"aluno: {dados[i][0]} nota 1: não cadastrada nota 2: {dados[i][2]}")
                            print()
        case 3:
        
            nome = input("qual aluno voce deseja excluir?").strip().lower()
            nd = str(input("voce deseja excluir tudo, nota 1 nota 2? [t, 1, 2] ")).strip().lower()
        
            match nd:
               
                case "t":
                    buscador(nome, True, False, False)        
               
                case '1':
                    buscador(nome, False, True, False)
               
                case '2':
                    buscador(nome, False, False, True)
        case 4:
            break
        
        
        
    
