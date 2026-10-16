from collections import defaultdict
from pathlib import Path

    ## carrega os valores pegados do arquivo 
def carregar_despesas(caminho: Path) -> defaultdict:
    despesas = defaultdict(float)
    if not caminho.exists():
        return despesas

    with open(caminho, "r", encoding="utf-8") as arquivo:
        for linha in arquivo:
            linha = linha.strip()
            if not linha or "," not in linha:
                continue

            cat, val = linha.split(",", 1)
            try:
                despesas[cat.strip()] += float(val.strip())
            except ValueError:
                continue
    return despesas

    ## escreve no arquivo com a lista de despesas criadas pelo user
def salvar_despesas(caminho: Path, despesas: dict):
    with open(caminho, "w", encoding="utf-8") as f:
        for nome, valor in sorted(despesas.items()):
            f.write(f"{nome}, {valor:.2f}\n")

if __name__ == "__main__":
    arquivo_entrada = Path("arquivo.txt")
    arquivo_saida = Path("saida.txt")

    despesas = carregar_despesas(arquivo_entrada)

    quant = int(input("Quantas despesas você quer cadastrar? "))
    for _ in range(quant):
        cat = input("Qual categoria: ").strip()
        val = float(input("Qual valor: "))
        despesas[cat] += val

    salvar_despesas(arquivo_saida, despesas)
    print(f"Despesas consolidadas salvas em {arquivo_saida.name}.")