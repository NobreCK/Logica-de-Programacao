import string
import secrets

def gerar_senha(tamanho: int) -> str:
    caracteres = string.punctuation + string.digits + string.ascii_letters
    return "".join(secrets.choice(caracteres) for i in range(tamanho))

if __name__ == "__main__":
    qtd = int(input("Tamanho da senha desejada: "))
    print(f"Senha gerada: {gerar_senha(qtd)}")