import string
from random import choice 

caracteres = string.punctuation + string.ascii_letters + string.digits
value = int(input("Quantas caracteres voce quer? "))

senha = "".join([choice(caracteres) for snh in range(value)])
