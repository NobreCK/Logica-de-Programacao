from pathlib import Path
import shutil 

def organizar_downloads():
    downloads = Path.home() / "Downloads"
    
    traducao = {
        "doc": "Documentos",
        "pdf": "Documentos",
        "png": "Imagens"
    }
    
    arquivos = [_ for _ in downloads.itedir() if _.is_file()]
    for i in arquivos:
        ext = i.suffix.lstrip(".").lower()
        if ext in traducao:
            pasta = Path.home / traducao[ext]
            pasta.mkdir(parents=True, exist_ok=True)
            shutil.move(str(i), str(pasta / i.name))
        
if __name__ == "__main__":
    organizar_downloads()