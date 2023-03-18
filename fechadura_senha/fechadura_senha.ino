#include <Keypad.h>  // Biblioteca do codigo

//PINOS

#define RELE 2
#define BOTAO 12

//
#define Password_Length 5  // tamanho da senha -1
char Data[Password_Length];
char Master[Password_Length] = "1144";  // coloque a senha aqui
byte data_count = 0, master_count = 0;
char tecla_pressionada;
bool digitando = false;

//Millis
unsigned long previousMillis = 0;
const long intervalo = 500;

const byte LINHAS = 4;   // Linhas do teclado
const byte COLUNAS = 3;  // Colunas do teclado

char TECLAS_MATRIZ[LINHAS][COLUNAS] = {  // Matriz de caracteres (mapeamento do teclado)
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte PINOS_LINHAS[LINHAS] = { 9, 8, 7, 6 };  // Pinos de conexao com as linhas do teclado
byte PINOS_COLUNAS[COLUNAS] = { 5, 4, 3 };   // Pinos de conexao com as colunas do teclado

Keypad teclado_principal = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);  // Inicia teclado

void setup() {
  Serial.begin(9600);  // Inicia porta serial

  pinMode(RELE, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BOTAO, INPUT);
  digitalWrite(RELE, HIGH);
}

void loop() {
  verificarSenha();  // função verifica se teclado é pressionado

  piscarLed();  // função pisca LED_BUILTIN quando a senha está sendo digitada

  //Serial.print(digitalRead(BOTAO));

  if (digitalRead(BOTAO) == HIGH) {  // verifica se botão de destravamento é pressionado

    
    Serial.print("botao pressionado");
    senhaCerta();
    delay(1000);
  }
}

void verificarSenha() {

  tecla_pressionada = teclado_principal.getKey();

  if (tecla_pressionada) {
    if (tecla_pressionada == '*') {  // verifica se * foi pressionado para apagar os digitos já inseridos e o usuario poder reiniciar a digitação da senha
      digitando = false;
      digitalWrite(LED_BUILTIN, 0);
      Serial.println("Digite sua senha novamente");
      clearData();
    } else {
      Data[data_count] = tecla_pressionada;
      Serial.println(tecla_pressionada);
      digitando = true;
      data_count++;
    }
  }

  if (data_count == Password_Length - 1) {  // executa quando a quantidade de digitos da senha esta na memoria

    if (!strcmp(Data, Master)) {
      Serial.println("Senha certa");
      digitando = false;
      senhaCerta();


    } else {
      Serial.println("Senha incorreta");
      delay(1000);
      digitando = false;
    }

    clearData();
  }
}

void clearData() {  // função limpa dados
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
}


void senhaCerta() {  // função liga rele (rele liga com GND na porta IN)

  digitalWrite(RELE, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(RELE, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
}

void piscarLed() {  // função pisca led quando digitando

  if (digitando) {
    unsigned currentMillis = millis();

    if (currentMillis - previousMillis >= intervalo) {
      previousMillis = currentMillis;

      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  }
}
