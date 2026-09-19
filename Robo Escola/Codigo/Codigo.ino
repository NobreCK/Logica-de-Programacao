#include <Bluepad32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>

// ==================== DISPLAY ====================
#define TFT_CS    10
#define TFT_RST   15
#define TFT_DC    16
#define TFT_MOSI  11
#define TFT_SCLK  12

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

#define FUNDO   ST7735_BLACK
#define BRANCO  ST7735_WHITE

// ==================== MOTORES ====================
const int MotorA_IA = 1;
const int MotorA_IB = 2;
const int MotorB_IA = 3;
const int MotorB_IB = 4;

const int freq = 5000;
const int resolution = 8;

// ==================== WIFI ====================
const char* ssid = "Fernanda Falcão";
const char* password = "erick2507";

WebServer server(80);

// ==================== CONTROLE ====================
GamepadPtr myGamepad;

enum EstadoRobo { PARADO, ANDANDO, FALANDO };
EstadoRobo estadoatual = PARADO;
EstadoRobo estadoAnterior = PARADO;

unsigned long ultimoOlhar = 0;
int faseOlhar = 0;

// ==================== HTML ====================
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Robo IA</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background: #1a1a2e;
      color: white;
      margin-top: 60px;
    }
    button {
      background: #4CAF50;
      color: white;
      border: none;
      padding: 22px 45px;
      font-size: 22px;
      border-radius: 14px;
      cursor: pointer;
    }
    button:active { background: #45a049; }
    #status {
      margin-top: 25px;
      font-size: 18px;
      min-height: 70px;
      padding: 0 18px;
      white-space: pre-wrap;
    }
  </style>
</head>
<body>
  <h1>Robo</h1>
  <button onclick="entrarModoIA()">Entrar no Modo IA</button>
  <div id="status">Aguardando...</div>

  <script>
    const GEMINI_API_KEY = "COLE A API";
    const MODELO = "gemini-2.0-flash";

    function entrarModoIA() {
      document.getElementById("status").innerText = "Entrando no Modo IA...";
      fetch("/modoia")
        .then(r => r.text())
        .then(data => {
          document.getElementById("status").innerText = data + "\nPode falar...";
          iniciarEscuta();
        })
        .catch(() => {
          document.getElementById("status").innerText = "Erro de conexao com o robo";
        });
    }

    const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
    let recognition = null;
    let textoFinal = "";

    if (SpeechRecognition) {
      recognition = new SpeechRecognition();
      recognition.lang = "pt-BR";
      recognition.continuous = false;
      recognition.interimResults = true;

      recognition.onstart = () => {
        document.getElementById("status").innerText = "Ouvindo... pode falar";
      };

      recognition.onresult = (event) => {
        let provisorio = "";
        for (let i = event.resultIndex; i < event.results.length; i++) {
          const t = event.results[i][0].transcript;
          if (event.results[i].isFinal) textoFinal += t + " ";
          else provisorio += t;
        }
        document.getElementById("status").innerText = "Ouvindo...\n" + (textoFinal + provisorio);
      };

      recognition.onend = async () => {
        if (textoFinal.trim() === "") {
          document.getElementById("status").innerText = "Nao entendi. Tente de novo.";
          return;
        }

        document.getElementById("status").innerText = "Pensando...";
        try {
          const resposta = await perguntarGemini(textoFinal.trim());
          document.getElementById("status").innerText = "Respondendo:\n" + resposta;
          falarResposta(resposta);
        } catch (e) {
          console.error(e);
          document.getElementById("status").innerText = "Erro ao falar com o Gemini";
          fetch("/parado");
        }
      };
    }

    function iniciarEscuta() {
      textoFinal = "";
      if (recognition) recognition.start();
    }

    async function perguntarGemini(texto) {
      const url = "https://generativelanguage.googleapis.com/v1beta/models/" + MODELO + ":generateContent?key=" + GEMINI_API_KEY;

      const res = await fetch(url, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          contents: [{ parts: [{ text: texto }] }]
        })
      });

      const data = await res.json();
      if (data.candidates && data.candidates[0]) {
        return data.candidates[0].content.parts[0].text;
      }
      throw new Error("Resposta invalida");
    }

    function falarResposta(texto) {
      fetch("/falando");

      const utterance = new SpeechSynthesisUtterance(texto);
      utterance.lang = "pt-BR";
      utterance.rate = 1.0;
      utterance.pitch = 1.0;

      utterance.onend = () => {
        fetch("/parado");
        document.getElementById("status").innerText = "Pronto. Pode falar de novo.";
      };

      speechSynthesis.speak(utterance);
    }
  </script>
</body>
</html>
)rawliteral";

// ==================== ROTAS ====================
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleModoIA() {
  server.send(200, "text/plain", "Modo IA ativado!");
}

void handleFalando() {
  estadoatual = FALANDO;
  server.send(200, "text/plain", "ok");
}

void handleParado() {
  estadoatual = PARADO;
  server.send(200, "text/plain", "ok");
}

// ==================== BLUEPAD ====================
void onconectado(GamepadPtr gp) {
  myGamepad = gp;
  Serial.println("Controle conectado");
}

void ondisconectado(GamepadPtr gp) {
  myGamepad = nullptr;
  Serial.println("Controle desconectado");
}

// ==================== DESENHO ====================
void desenharOlhos(int xEsq, int xDir, int y, int raio) {
  tft.fillRect(xEsq - raio - 4, y - raio - 2, (xDir - xEsq) + raio * 2 + 8, raio * 2 + 4, FUNDO);
  tft.fillCircle(xEsq, y, raio, BRANCO);
  tft.fillCircle(xDir, y, raio, BRANCO);
}

void desenharBocaPequena() {
  tft.fillRoundRect(70, 90, 20, 7, 3, BRANCO);
}

// ==================== CARAS ====================
void caraParada() {
  if (millis() - ultimoOlhar > 1600) {
    ultimoOlhar = millis();
    faseOlhar = (faseOlhar + 1) % 4;

    tft.fillScreen(FUNDO);
    desenharBocaPequena();

    if (faseOlhar == 0 || faseOlhar == 2) {
      desenharOlhos(50, 110, 45, 14);
    } else if (faseOlhar == 1) {
      desenharOlhos(40, 100, 45, 14);
    } else {
      desenharOlhos(60, 120, 45, 14);
    }
  }
}

void caraAndando() {
  tft.fillScreen(FUNDO);
  desenharOlhos(50, 110, 45, 14);
  desenharBocaPequena();
}

void caraFalando() {
  tft.fillScreen(FUNDO);
  tft.fillCircle(50, 45, 14, BRANCO);
  tft.fillCircle(110, 45, 14, BRANCO);

  for (int i = 0; i < 5; i++) {
    tft.fillRoundRect(65, 85, 30, 16, 7, BRANCO);
    delay(90);
    tft.fillRoundRect(65, 85, 30, 16, 7, FUNDO);
    tft.fillRoundRect(70, 90, 20, 7, 3, BRANCO);
    delay(90);
  }
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== INICIOU ===");

ledcSetup(0, freq, resolution);
ledcAttachPin(MotorA_IA, 0);

ledcSetup(1, freq, resolution);
ledcAttachPin(MotorA_IB, 1);

// Canal 2 e 3 - Motor B
ledcSetup(2, freq, resolution);
ledcAttachPin(MotorB_IA, 2);

ledcSetup(3, freq, resolution);
ledcAttachPin(MotorB_IB, 3);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(FUNDO);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  int t = 0;
  while (WiFi.status() != WL_CONNECTED && t < 20) {
    delay(500);
    Serial.print(".");
    t++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha no WiFi");
  }

  server.on("/", handleRoot);
  server.on("/modoia", handleModoIA);
  server.on("/falando", handleFalando);
  server.on("/parado", handleParado);
  server.begin();
  Serial.println("Servidor iniciado");

  BP32.setup(&onconectado, &ondisconectado);

  estadoatual = PARADO;
}

// ==================== LOOP ====================
void loop() {
  BP32.update();
  server.handleClient();

  if (estadoatual != estadoAnterior) {
    tft.fillScreen(FUNDO);
    estadoAnterior = estadoatual;
    ultimoOlhar = 0;
  }

  if (estadoatual == PARADO) {
    caraParada();
  } else if (estadoatual == ANDANDO) {
    caraAndando();
  } else if (estadoatual == FALANDO) {
    caraFalando();
  }

  if (myGamepad && myGamepad->isConnected()) {
    int eixoY = myGamepad->axisY();
    int eixoX = myGamepad->axisX();

    if (abs(eixoY) < 25 && abs(eixoX) < 25) {
      ledcWrite(MotorA_IA, 0);
      ledcWrite(MotorA_IB, 0);
      ledcWrite(MotorB_IA, 0);
      ledcWrite(MotorB_IB, 0);
      if (estadoatual != FALANDO) estadoatual = PARADO;
      return;
    }

    estadoatual = ANDANDO;

    int velocidade = constrain(map(abs(eixoY), 25, 512, 0, 255), 0, 255);
    int ajuste = constrain(map(abs(eixoX), 25, 512, 0, 255), 0, 255);

    int velEsq = velocidade;
    int velDir = velocidade;

    if (eixoX < -25) {
      velEsq = max(0, velEsq - ajuste);
    } else if (eixoX > 25) {
      velDir = max(0, velDir - ajuste);
    }

    if (eixoY < -25) {
      ledcWrite(MotorA_IA, velEsq); ledcWrite(MotorA_IB, 0);
      ledcWrite(MotorB_IA, velDir); ledcWrite(MotorB_IB, 0);
    } else if (eixoY > 25) {
      ledcWrite(MotorA_IA, 0); ledcWrite(MotorA_IB, velEsq);
      ledcWrite(MotorB_IA, 0); ledcWrite(MotorB_IB, velDir);
    }
  }
}