<<<<<<< HEAD
import string
import secrets

def gerar_senha(tamanho: int) -> str:
    caracteres = string.punctuation + string.digits + string.ascii_letters
    return "".join(secrets.choice(caracteres) for i in range(tamanho))

if __name__ == "__main__":
    qtd = int(input("Tamanho da senha desejada: "))
    print(f"Senha gerada: {gerar_senha(qtd)}")
=======
import string
from random import choice 

caracteres = string.punctuation + string.ascii_letters + string.digits
value = int(input("Quantas caracteres voce quer? "))

senha = "".join([choice(caracteres) for snh in range(value)])
>>>>>>> d20dd6d785e51bd42e81accbdf74a040799cf54f
