with open("arquivo", "r", enconding="utf-8") as arquivo:
    t = 0
    for i in arquivo:
        linha = i.split(",")
        
        cat, quantidade, valores = linha[0], float(linha[1]), float(linha[2])
        dicionario[cat] = quantidade, valores
    
    for j in dicionario.values():
        t += (j[0] * j[1])
    
    print(f"o total e de {t} e a lista {dicionario}")
    es = input("voce quer adicionar mais algo? se sim [produto, quantidade, valor]")
    sep = es.split(",")
    
    try:
        cat, quantidade, valores = sep[0], float(sep[1]), float(sep[2])
        dicionario[cat] = (quantidade, valores)
        
    except ValueError:
        return
    
    with open("arquivo", "w", enconding="utf-8") as saida:
        for i, j in dicionario.items():
            saida.write(f"{i},{j[0]},{j[1]}\n")
        saida.write(f"{t}")