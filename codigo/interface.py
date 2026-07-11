import tkinter as tk
import threading
from tkinter import filedialog, messagebox
import os

from main import main

caminho_arquivo = ""

def selecionar_arquivo():
    global caminho_arquivo
    
    caminho = filedialog.askopenfilename(
        title='Seleione o arquivo de produtos',
        filetypes=[('Arquivos de Texto', '*.txt')]
    )
    
    if caminho:
        caminho_arquivo = caminho
        lbl_arquivo.config(text=f"Arquivo selecionado: {caminho_arquivo}")  

def rodar():
    if not caminho_arquivo:
        messagebox.showerror("Erro", "Selecione um arquivo antes de rodar.")
        return
    
    thread_robo = threading.Thread(target=main, args=(caminho_arquivo,))
    thread_robo.start()
    
def abrir_log():
    try:
        os.startfile("log_erro.txt")
    except FileNotFoundError:
        messagebox.showerror('Aviso', 'Arquivo de log não encontrado.')

janela = tk.Tk()
janela.geometry("400x200")
janela.title("Alocacao de produtos")


titulo = tk.Label(janela, text="Alocacao de produtos", font=("Arial", 16, "bold"))
titulo.pack(pady=10)

botao_arquivo = tk.Button(janela, text="Selecionar Arquivo", command=selecionar_arquivo, font=("Arial", 12))
botao_arquivo.pack(pady=10)

lbl_arquivo = tk.Label(janela, text="Nenhum arquivo selecionado", font=("Arial", 10))
lbl_arquivo.pack(pady=10)

btn_iniciar = tk.Button(janela, text="iniciar automacao", font=("Arial", 10, "bold"), bg="green", fg="white", command=rodar)
btn_iniciar.pack(pady=15)

btn_log = tk.Button(janela, text="📄 Abrir Log de Erros", command=abrir_log)
btn_log.pack(pady=5)

janela.mainloop()
        