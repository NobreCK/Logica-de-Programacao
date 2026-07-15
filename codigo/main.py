import sys
import os
from selenium import webdriver
import time
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.keys import Keys

driver = None
wait = None

def ler_arquivo(caminho):
    produtos = []
    if not os.path.exists(caminho):
        return[]
    try:
        with open(caminho, 'r', encoding='utf-8') as f:
            linhas = f.readlines()

            # Pula a primeira linha (cabeçalho) com segurança
            # O slicing [1:] faz isso sem precisar do next(f)
            for linha in linhas[1:]:
                linha = linha.strip()
                if not linha: continue  # Pula linhas em branco
                
                dados = linha.split(';')
                if len(dados) == 3:
                    produtos.append({
                        'codigo': dados[0],
                        'prateleira': dados[1],
                        'nome': dados[2]
                    })
    except FileNotFoundError:
        print(f"Erro: O arquivo '{caminho}' não foi encontrado.")
        return []
    
    return produtos
# produtos = [(codigo, prateleira, nome), (codigo, prateleira, nome)] 


def ler_nome():
    return wait.until(
        EC.presence_of_element_located((By.XPATH, '//*[@id="span_W0032W0081E18DESCRI"]')
        )
    ).text
## retorna o txt do elemento alocado na tal localizacao dada


def erro_produto():
    erro = driver.find_elements( 
        By.XPATH,
        '//*[@id="W0032W0081E18CODPRO_Balloon"]'
    )
    if erro and erro[0].is_displayed(): 
        return erro[0].text
    # erro = [elemento]
    return None ## se nao achou erro nao retorna nada


def clicar_mais():
    wait.until(
        EC.element_to_be_clickable((By.XPATH, '//*[@id="W0032INSERIR"]'))
    ).click()


def erro_prateleira():
    erro = driver.find_elements(
        By.XPATH,
        '//*[@id="W0032W0081E70PRATEL_Balloon"]'
    )

    if erro and erro[0].is_displayed():
        return erro[0].text

    return None


def salvar():
    wait.until(
        EC.element_to_be_clickable((By.XPATH, '//*[@id="W0032W0081BTNTRN_ENTER"]')
        )
    ).click()


def fechar():
    wait.until(
        EC.element_to_be_clickable((By.XPATH, '//*[@id="W0032W0081BTNFECHAR"]')
        )
    ).click()


def escrever(xpath, texto):
    campo = wait.until(EC.element_to_be_clickable((By.XPATH, xpath)))
    campo.click()
    campo.clear()
    campo.send_keys(texto)
    campo.send_keys(Keys.TAB)

def salvar_log(codigo, prateleira, nome, tipo_erro, mensagem):
    with open('log_erro.txt', 'a', encoding='utf-8') as f:
        f.write(f'ERRO {tipo_erro:<11} | {mensagem}\n')
        f.write(f'CÓDIGO: {codigo} | PRATELEIRA: {prateleira} | NOME: {nome}\n')
        f.write('-' * 60 + '\n')


def main(caminho):
    global produtos, driver, wait

    print("Conectando ao Chrome...")
    options = webdriver.ChromeOptions()
    options.add_experimental_option("debuggerAddress", "127.0.0.1:9222")


    driver = webdriver.Chrome(options=options)
    wait = WebDriverWait(driver, 10)
    print("Conectado com sucesso!") 
    
    
    produtos = ler_arquivo(caminho)
    for item in produtos:
        # Extraindo os dados do dicionário
        codigo = item['codigo']
        prateleira = item['prateleira']
        nome = item['nome']
        
        clicar_mais()
        time.sleep(2)

        # Usando sua nova função de escrever
        escrever('//*[@id="W0032W0081E18CODPRO"]', codigo)
        time.sleep(2)

        # Verifica erros do produto
        erro = erro_produto()
        if erro:
            salvar_log(codigo, prateleira, nome, 'PRODUTO', erro)
            fechar()
            continue
        
        # Verifica o nome
        nome_site = ler_nome()
        if nome_site.upper() not in nome.upper():
            salvar_log(codigo, prateleira, f"{nome_site} (esperado: {nome})", 'NOME', "Nome divergente")
            fechar()
            continue
        
        time.sleep(2)
    
        # Usando a nova função para a prateleira
        escrever('//*[@id="W0032W0081E70PRATEL"]', prateleira)
        
        # Verifica erros da prateleira
        erro1 = erro_prateleira()
        if erro1:
            salvar_log(codigo, prateleira, nome, 'PRATELEIRA', erro1)
            fechar()
            continue
        
        time.sleep(2)
    
        # Salva
        salvar()

