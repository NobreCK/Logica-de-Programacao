import requests

def conversor_moedas():
   
    wallet = float(input("Quanto dinheiro você tem na carteira em dólar? "))
    
    url = "https://economia.awesomeapi.com.br/last/USD-BRL"
    
    try:
        resposta = requests.get(url)
        dados = resposta.json()
        
        cotacao_dolar = float(dados["USDBRL"]["bid"])
        resultado = wallet * cotacao_dolar
        
        print("Cotação: R$ {:.2f}".format(cotacao_dolar))
        print("Total na carteira em reais: R$ {:.2f}".format(resultado))

    except ValueError:
        print("Erro: Por favor, digite apenas números.")
    
    except Exception as e:
        print(f"Erro na requisição do valor atual do dólar: {e}")
        
conversor_moedas()
