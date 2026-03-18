import requests

def conversor_moedas():
   
    wallet = float(input("Quanto dinheiro você tem na carteira em dólar? "))
    
    url = "https://economia.awesomeapi.com.br/last/USD-BRL"
    
    try:
        resposta = requests.get(url)
        dados = resposta.json()
        
        cotacao_dolar = float(dados["USDBRL"]["bid"])
        resultado = wallet * cotacao_dolar
        
        print(f"Cotação: R$ {cotacao_dolar:.2f}")
        print(f"Total na carteira em reais: R$ {resultado:.2f}")

    except ValueError:
        print("Erro: Por favor, digite apenas números.")
    
    except Exception as e:
        print(f"Erro na requisição do valor atual do dólar: {e}")
        
conversor_moedas()