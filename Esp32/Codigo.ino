#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>

#define DHTPIN 1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


#define TFT_CS   10
#define TFT_RST 15
#define TFT_DC   16 
#define TFT_MOSI 11 
#define TFT_SCLK 12


#define FLIPPER_ORANGE 0xFD20 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const int btnUp = 4;
const int btnSelect = 5;
const int btnDown = 6;

const int AJUSTE_CENTRO = 2; 

int itemselecionado = 0;   
int totalitens = 5; 

enum EstadoTela { PRINCIPAL, REDES, INFRAVERMELHO, CLIMA };
EstadoTela estadoatual = PRINCIPAL;

const char* menuRedes[] = {"Derrubar e clonar", "Derrubar", "Jammer", "Captura de info com portal de login", "Voltar"};
const char* menuInfravermelho[] = {"Desligar", "Ligar", "Outros", "Voltar"};

const unsigned char icone_antena_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x01, 0x90, 0x09, 0x80, 0x03, 0x30, 0x0C, 0xC0, 0x06, 0x63, 0xC6, 0x60, 0x04, 0xC7, 0xE3, 0x20, 0x0C, 0xCF, 0xF3, 0x30,
  0x0C, 0xCF, 0xF3, 0x30, 0x0C, 0xCF, 0xF3, 0x30, 0x04, 0xC7, 0xE3, 0x20, 0x06, 0x63, 0xC6, 0x60, 0x03, 0x30, 0x0C, 0xC0, 0x01, 0x91, 0x89, 0x80, 0x00, 0xC3, 0xC3, 0x00, 0x00, 0x43, 0xC2, 0x00,
  0x00, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x0D, 0xB0, 0x00, 0x00, 0x09, 0x90, 0x00, 0x00, 0x11, 0x88, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x38, 0x1C, 0x00,
  0x00, 0x70, 0x0E, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0xE1, 0x87, 0x00, 0x00, 0xC1, 0x83, 0x00,   0x01, 0xC3, 0xC3, 0x80, 0x01, 0x87, 0xE1, 0x80, 0x03, 0x9F, 0xF9, 0xC0, 0x03, 0xFF, 0xFF, 0xC0,

};

const unsigned char icone_wifi_32x32[] PROGMEM = {
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x0c, 0x1f, 0xf8, 0x30, 0x18, 0x7f, 0xfe, 0x18, 0x10, 0xf0, 0x0f, 0x08,
  0x00, 0xc0, 0x03, 0x00, 0x01, 0x87, 0xe1, 0x80, 0x03, 0x0f, 0xf0, 0xc0, 0x02, 0x1c, 0x38, 0x40, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00,
  0x00, 0x06, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char icone_clima_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0xFF, 0x00,  0x00, 0x01, 0x00, 0x80,  0x00, 0x02, 0x00, 0x40,
  0x00, 0x04, 0x00, 0x20,  0x01, 0xFC, 0x00, 0x20,  0x0F, 0xFC, 0x00, 0x20,  0x1F, 0xFC, 0x00, 0x20, 0x3F, 0xFF, 0x00, 0x20,  0x3F, 0xFF, 0xF0, 0x60,  0x3F, 0xFF, 0xFF, 0x40,  0x7F, 0xFF, 0xFF, 0xFC, 
  0x7F, 0xFF, 0xFF, 0xFE,  0x7F, 0xFF, 0xFF, 0xFE,  0x3F, 0xFF, 0xFF, 0xFE,  0x3F, 0xFF, 0xFF, 0xFE, 0x3F, 0xFF, 0xFF, 0xFE,  0x1F, 0xFF, 0xFF, 0xFE,  0x0F, 0xFF, 0xFF, 0xFC,  0x07, 0xFF, 0xFF, 0xFC,
  0x01, 0xFF, 0xFF, 0xF8,  0x00, 0x7F, 0xFF, 0xF0,  0x00, 0x07, 0xFC, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
};

const unsigned char icone_ia_32x32[] PROGMEM = {
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0xc0, 0x03, 0x80,
  0x03, 0x98, 0x19, 0xc0, 0x03, 0x3c, 0x3c, 0xc0, 0x03, 0x3c, 0x3c, 0xc0, 0x03, 0x18, 0x18, 0xc0, 0x03, 0x00, 0x00, 0xc0, 0x03, 0x3f, 0xfc, 0xc0, 0x03, 0x00, 0x00, 0xc0, 0x01, 0x80, 0x01, 0x80, 
  0x01, 0x3c, 0x3c, 0x80, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char icone_rf_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x01, 0x98, 0x19, 0x80, 0x03, 0x30, 0x0C, 0xC0, 0x06, 0x60, 0x06, 0x60, 0x0C, 0xC7, 0xE3, 0x30, 0x0C, 0xCF, 0xF3, 0x30, 0x0C, 
  0xCF, 0xF3, 0x30, 0x0C, 0xCF, 0xF3, 0x30, 0x0C, 0xCF, 0xF3, 0x30, 0x0C, 0xCF, 0xF3, 0x30, 0x0C, 0xC7, 0xE3, 0x30, 0x0C, 0xC0, 0x03, 0x30, 0x06, 0x61, 0x86, 0x60, 0x06, 0x33, 0xCC, 0x60, 0x03, 0x03, 
  0xC0, 0xC0, 0x01, 0x87, 0xE1, 0x80, 0x00, 0xC7, 0xE3, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x0F, 0xF0, 0x00,  0x00, 0x1F, 0xF8, 0x00,0x00, 0x1F, 0xF8, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x3F, 0xFC, 
  0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void carregarmenus();
void clima();
void ia();
void rf();
void iniciarTopo();
void desenharInterfaceCarrossel();
void printCentro(const char* texto, int y, int tamanho, bool altura, bool sublinhar);
void desenharIconeComEscala(int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h, uint16_t col, uint8_t escala);
void limparEsquerdaParaDireita(uint16_t corDeFundo);
void limparDireitaparaEsquerda(uint16_t corDeFundo);

void setup() {
    pinMode(btnUp, INPUT_PULLUP);
    pinMode(btnSelect, INPUT_PULLUP);
    pinMode(btnDown, INPUT_PULLUP);
    pinMode(DHTPIN, INPUT);
    dht.begin();

    WiFi.mode(WIFI_AP_STA);

    tft.initR(INITR_BLACKTAB); 
    tft.setRotation(1); 
    tft.setTextSize(1); 
    
    tft.fillScreen(ST7735_BLACK); 
    
    iniciarTopo();
    desenharInterfaceCarrossel();
}

void loop() {
    if (digitalRead(btnUp) == LOW) {
        itemselecionado--;
        if (itemselecionado < 0) {
            itemselecionado = totalitens - 1; 
        }
        
        if (estadoatual == PRINCIPAL) {
            limparDireitaparaEsquerda(ST7735_BLACK); 
            desenharInterfaceCarrossel();
        } else {
            carregarmenus();
        }
        
        delay(50);
        while (digitalRead(btnUp) == LOW);
    }

    if (digitalRead(btnDown) == LOW) {
        itemselecionado++;
        if (itemselecionado >= totalitens) {
            itemselecionado = 0; 
        }

        if (estadoatual == PRINCIPAL) {
            limparEsquerdaParaDireita(ST7735_BLACK); 
            desenharInterfaceCarrossel(); 
        } else {
            carregarmenus();
        }
        
        delay(50);
        while (digitalRead(btnDown) == LOW);
    }
                                       // BOTAR AS ACOES OU CHAMADAS DE INTERFACES AQUI
    if (digitalRead(btnSelect) == LOW) {
        if (estadoatual == PRINCIPAL) {                   
            if (itemselecionado == 0) {estadoatual = INFRAVERMELHO; totalitens = 4; itemselecionado = 2; tft.fillScreen(ST7735_BLACK); iniciarTopo(); carregarmenus(); return;}
            else if (itemselecionado == 1) {estadoatual = REDES; totalitens = 5; itemselecionado = 2; tft.fillScreen(ST7735_BLACK); iniciarTopo(); carregarmenus(); return;}
            else if (itemselecionado == 2) {clima(); estadoatual = CLIMA; return;}
            else if (itemselecionado == 3) {ia(); return;}
            else if (itemselecionado == 4) {rf(); return;}
        } else {

            if ((estadoatual == REDES && itemselecionado == 4) || (estadoatual == INFRAVERMELHO && itemselecionado == 3) || (estadoatual == CLIMA && itemselecionado == 1)) {
                estadoatual = PRINCIPAL;
                totalitens = 5;
                itemselecionado = 0;
                tft.fillScreen(ST7735_BLACK);
                iniciarTopo();
                desenharInterfaceCarrossel();
                return;
            }
            if (estadoatual == REDES) { // funcoes que vai executar 
                if (itemselecionado == 1) {listar(); return;}
            }

        return;
        }
        delay(300);

        limparEsquerdaParaDireita(ST7735_BLACK); 
        iniciarTopo();                
        desenharInterfaceCarrossel(); 
        while (digitalRead(btnSelect) == LOW);
    }
}

void iniciarTopo() {
    tft.fillRect(6, 6, tft.width() - 12, 10, 0x0115); 
    tft.setTextColor(ST7735_WHITE);
    printCentro("NOBRE", 7, 1, false, false);

    int ySetaCentro = tft.height() / 2 - 4; 
    int xSetaDireita = tft.width() - 18;      

    tft.setTextColor(ST7735_BLUE);
    tft.setCursor(12, ySetaCentro);           tft.print("<");
    tft.setCursor(xSetaDireita, ySetaCentro); tft.print(">");

    tft.drawRect(5, 5, tft.width() - 10, tft.height() - 10, ST7735_BLUE);
}

void desenharInterfaceCarrossel() {
    int xBaseCentro = (tft.width() / 2) - 32 + AJUSTE_CENTRO;  
    int yBaseCentro = (tft.height() / 2) - 32;      
    int y = 106;

    tft.setTextColor(ST7735_WHITE);

    switch (itemselecionado) {
        case 0:
            desenharIconeComEscala(xBaseCentro, yBaseCentro, icone_antena_32x32, 32, 32, ST7735_CYAN, 2);
            printCentro("INFRAVERMELHO", y, 1, false, false); 
            break;

        case 1:
            desenharIconeComEscala(xBaseCentro, yBaseCentro + 10, icone_wifi_32x32, 32, 32, ST7735_CYAN, 2);
            printCentro("WIFI", y, 1, false, false);
            break;

        case 2:
            desenharIconeComEscala(xBaseCentro + 1, yBaseCentro + 6, icone_clima_32x32, 32, 32, ST7735_CYAN, 2);
            printCentro("CLIMA", y, 1, false, false);
            break;

        case 3:
            desenharIconeComEscala(xBaseCentro + 1, yBaseCentro + 8, icone_ia_32x32, 32, 32, ST7735_CYAN, 2);
            printCentro("IA", y, 1, false, false);
            break;

        case 4:
            desenharIconeComEscala(xBaseCentro + 1, yBaseCentro + 8, icone_rf_32x32, 32, 32, ST7735_CYAN, 2);
            printCentro("RF", y, 1, false, false);
            break;
    }
}

void printCentro(const char* texto, int y, int tamanho, bool altura, bool sublinhar) {
    int comprimento = strlen(texto);
    int larguraTexto = comprimento * (6 * tamanho); 

    int x = (tft.width() / 2) - (larguraTexto / 2) + AJUSTE_CENTRO;

    int alturatexto = 8 * tamanho; 

    if (altura) {
        y = (tft.height() / 2) - (alturatexto / 2);
    }

    tft.setTextSize(tamanho);
    tft.setCursor(x, y);
    tft.print(texto);

    if (sublinhar) {
        int yLinha = y + alturatexto + 1;
        tft.drawFastHLine(x, yLinha, larguraTexto, ST7735_WHITE);
    }
    tft.setTextSize(1); 
}

void carregarmenus() {
    const char* const* menuItens;
    
    switch (estadoatual) {
        case REDES:
            menuItens = menuRedes;
            break;
        case INFRAVERMELHO:
            menuItens = menuInfravermelho;
            break;
        default:
            return;
    }
    tft.fillRect(6, 18, tft.width() - 12, tft.height() - 28, ST7735_BLACK); // ✅ limpa só a área do conteúdo
    for (int i = 0; i < totalitens; i++) {
        int distancia = i - itemselecionado;

        if (i == itemselecionado) {
            tft.setTextColor(ST7735_WHITE); 
            printCentro(menuItens[i], 0, 2, true, true);
        }
        else {    
            int ycalculado = (tft.height() / 2) + (distancia * 24); 
            
            ycalculado = ycalculado - 4;

            if (ycalculado > 15 && ycalculado < 115) {
                tft.setTextColor(0x7BEF); 
                printCentro(menuItens[i], ycalculado, 1, false, false); 
            }
        }
    }
}

void desenharIconeComEscala(int16_t x, int16_t y, const unsigned char *bitmap, int16_t w, int16_t h, uint16_t col, uint8_t escala) {
    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t b = 0;

    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            if (i & 7) b <<= 1;
            else b = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
            
            if (b & 0x80) {
                tft.fillRect(x + (i * escala), y + (j * escala), escala, escala, col);
            }
        }
    }
}

void limparEsquerdaParaDireita(uint16_t corDeFundo) {
    int xBaseCentro = (tft.width() / 2) - 32 + AJUSTE_CENTRO;  
    int yBaseCentro = (tft.height() / 2) - 32; 
    int larguraFatia = 4;

    for (int fx = 0; fx < 80; fx += larguraFatia) {
        tft.fillRect(xBaseCentro - 10 + fx, yBaseCentro + 2, larguraFatia, 84, corDeFundo); 
           
    }
}

void limparDireitaparaEsquerda(uint16_t corDeFundo) {
    int xBaseCentro = (tft.width() / 2) - 32;  
    int yBaseCentro = (tft.height() / 2) - 32; 
    int largurafatia = 4;

    for (int fx = 0; fx < 80; fx += largurafatia) {     
        tft.fillRect(xBaseCentro + 70 - fx, yBaseCentro + 2, largurafatia, 84, corDeFundo);
    }
}

void clima() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    tft.fillScreen(ST7735_BLACK);
    iniciarTopo();

    tft.setTextColor(ST7735_CYAN);
    printCentro("CLIMA", 20, 1, false, false);
    tft.drawFastHLine(20, 30, tft.width() - 40, ST7735_CYAN);

    tft.setTextColor(0xFD20);
    printCentro("TEMP", 40, 1, false, false);
    tft.setTextColor(ST7735_WHITE);
    char bufT[10];
    dtostrf(t, 4, 1, bufT);
    strcat(bufT, " C");
    printCentro(bufT, 52, 2, false, false);

    tft.drawFastHLine(20, 74, tft.width() - 40, 0x7BEF);

    tft.setTextColor(0x7BEF);
    printCentro("UMIDADE", 80, 1, false, false);
    tft.setTextColor(ST7735_WHITE);
    char bufH[10];
    dtostrf(h, 4, 1, bufH);
    strcat(bufH, " %");
    printCentro(bufH, 92, 2, false, false);

    itemselecionado = 1;
}


void listar() {
    const char* menuItens[10];
    int n = WiFi.scanNetworks();
    if (n == 0) {
        tft.setCursor(0, 0);
        tft.println("Nenhuma rede encontrada");
        return;
    } else {

        int max = (n > 10) ? 10 : n;

        for (int i = 0; i < max; i++) {
            menuItens[i] = WiFi.SSID(i).c_str();
        }
        
        tft.fillScreen(ST7735_BLACK);

        for (int i = 0; i < max; i++) {
            if (i == itemselecionado) {
                printCentro(menuItens[i], i * 16 + 6, 1, false, true);
            } else {
                printCentro(menuItens[i], i * 16 + 6, 1, false, false);
            }
        }
    }

}

/// IA FUNCAO, USAR NO ROBO TAMBEM
const char* ssid = "Fernanda Falcão";
const char* password = "erick2507";
// ==================================

WebServer server(80);
bool modoIA = false;

// Página HTML
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Robô IA</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background: #1a1a2e;
      color: white;
      margin-top: 80px;
    }
    button {
      background: #4CAF50;
      color: white;
      border: none;
      padding: 25px 50px;
      font-size: 24px;
      border-radius: 15px;
      cursor: pointer;
    }
    button:active {
      background: #45a049;
    }
    #status {
      margin-top: 30px;
      font-size: 20px;
      min-height: 60px;
      padding: 0 20px;
    }
  </style>
</head>
<body>
  <h1>Robô</h1>
  <button id="btnModoIA" onclick="entrarModoIA()">Entrar no Modo IA</button>
  <div id="status">Aguardando...</div>

  <script>
    // ===== 1. Avisa o ESP32 que entrou no modo IA =====
    function entrarModoIA() {
      document.getElementById("status").innerText = "Entrando no Modo IA...";

      // Avisa o ESP32
      fetch("/modoia")
        .then(r => r.text())
        .then(data => {
          document.getElementById("status").innerText = data + "\nPode falar...";
          iniciarEscuta();   // começa a ouvir
        })
        .catch(() => {
          document.getElementById("status").innerText = "Erro de conexão com o robô";
        });
    }

    // ===== 2. Configuração do reconhecimento de voz =====
    const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;

    if (!SpeechRecognition) {
      document.getElementById("status").innerText = "Seu navegador não suporta reconhecimento de voz";
    } else {
      const recognition = new SpeechRecognition();
      recognition.lang = "pt-BR";
      recognition.continuous = false;        // para quando a pessoa para de falar
      recognition.interimResults = true;     // mostra o que está sendo falado em tempo real

      let textoFinal = "";

      // Quando começa a ouvir
      recognition.onstart = () => {
        document.getElementById("status").innerText = "Ouvindo... pode falar";
      };

      // Enquanto a pessoa está falando (e quando termina)
      recognition.onresult = (event) => {
        let textoProvisorio = "";

        for (let i = event.resultIndex; i < event.results.length; i++) {
          const resultado = event.results[i][0].transcript;

          if (event.results[i].isFinal) {
            textoFinal += resultado + " ";
          } else {
            textoProvisorio += resultado;
          }
        }

        // Mostra na tela o que está sendo entendido
        document.getElementById("status").innerText = 
          "Ouvindo...\n" + (textoFinal + textoProvisorio);
      };

      // Quando a pessoa para de falar
      recognition.onend = () => {
        if (textoFinal.trim() !== "") {
          document.getElementById("status").innerText = "Você disse:\n" + textoFinal;
          
          // Aqui depois a gente manda o textoFinal pro Gemini
          // e faz o robô falar a resposta
          
          // Por enquanto só mostra o que ouviu
          console.log("Texto final:", textoFinal);
        } else {
          document.getElementById("status").innerText = "Não entendi. Tente de novo.";
        }
      };

      // Função que inicia a escuta
      function iniciarEscuta() {
        textoFinal = "";
        recognition.start();
      }
    }

    // ===== 3. Função pra falar (Text-to-Speech) =====
    // Depois a gente usa essa função pra falar a resposta do Gemini
    function falar(texto) {
      const utterance = new SpeechSynthesisUtterance(texto);
      utterance.lang = "pt-BR";
      utterance.rate = 1;
      utterance.pitch = 1;
      speechSynthesis.speak(utterance);
    }
  </script>
</body>
</html> )rawliteral";

void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

void handleModoIA() {
    modoIA = true;
    Serial.println(">>> ENTROU NO MODO IA <<<");
    tft.fillScreen(ST7735_BLACK);
    server.send(200, "text/plain", "Modo IA ativado!");
}

void ia() {
    tft.fillScreen(ST7735_BLACK);
    Serial.println("Entrando no Modo IA...");
    tft.setCursor(0, 0);

    tft.println("Conectando no WiFi...");
    tft.setCursor(0, 10);
    WiFi.begin(ssid, password);

    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
        delay(500);
        tft.print(".");
        tentativas++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        tft.println("\nConectado!");
        tft.print("IP do ESP32: ");
        tft.println(WiFi.localIP());
    } else {
        tft.println("\nFalha ao conectar no WiFi");
        return; 
    }

    server.on("/", handleRoot);
    server.on("/modoia", handleModoIA);
    server.begin();
    tft.println("Servidor iniciado");

  // 4. Loop do Modo IA (fica aqui até você decidir sair)
    while (true) {
        server.handleClient();

    // Aqui depois você pode colocar:
    // - verificar se quer sair do modo IA
    // - ler sensores
    // - etc

    // Exemplo de como sair no futuro:
    // if (botaoSairPressionado) break;
    }
}

////////////////////////////////////////
void rf() {
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(30, 60);
    tft.setTextColor(ST7735_WHITE);
    tft.print("Clima"); // ✅
}

