# VoltSwitch

Firmware para ESP32 que liga ou desliga uma máquina conforme uma agenda de horas.

O dispositivo se conecta ao Wi-Fi, sincroniza o relógio via NTP, verifica se a máquina deve estar online no horário atual e compara isso com o estado reportado por uma API HTTP. Quando a máquina deveria estar online e não responde, o firmware envia um pacote Wake-on-LAN. Quando ela está online fora da agenda, o firmware chama o endpoint de desligamento. Depois de cada verificação, o ESP32 entra em deep sleep até a próxima mudança relevante de agenda.

## Funcionamento

1. O ESP32 carrega as variáveis do arquivo `.env` durante o build.
2. No boot, configura o fuso horário e prepara a lista de horas agendadas.
3. No loop principal:
   - conecta ao Wi-Fi, se necessário;
   - obtém a hora local via NTP;
   - consulta `API_URL` com `GET` para verificar saúde;
   - envia Wake-on-LAN quando a máquina deveria estar ligada;
   - envia `POST` para `API_URL/shutdown` quando deveria estar desligada;
   - calcula o próximo horário de interesse e entra em deep sleep.

## Requisitos

- ESP32 compatível com a placa `esp32dev`.
- PlatformIO instalado.
- Rede Wi-Fi com acesso à máquina alvo.
- Máquina alvo configurada para Wake-on-LAN.
- Serviço HTTP na máquina alvo respondendo:
  - `GET /` ou `GET API_URL` com status `2xx` quando saudável;
  - `POST /shutdown` para solicitar desligamento.

## Configuração

Crie o arquivo `.env` a partir do exemplo:

```sh
cp .env.example .env
```

Edite os valores conforme o ambiente:

```env
# General
TZ="BRT3"
HOURS="7,8,11,12,15,16,19,20"

# API
API_URL="http://192.168.1.50:3939"

# Wi-Fi
WIFI_SSID="wifi_ssid"
WIFI_PASS="wifi_password"

# Wake-on-LAN
WOL_MAC_ADDRESS="00:00:00:00:00:00"
WOL_BROADCAST_IP="255.255.255.255"
WOL_PORT="9"
```

### Variáveis

| Variável | Descrição |
| --- | --- |
| `TZ` | Fuso horário usado por `configTzTime`. Para horário de Brasília sem horário de verão, use `BRT3`. |
| `HOURS` | Lista de horas inteiras, separadas por vírgula, em que a máquina deve permanecer ligada. Valores válidos: `0` a `23`. |
| `API_URL` | URL base da API de saúde/desligamento da máquina alvo. |
| `WIFI_SSID` | Nome da rede Wi-Fi. |
| `WIFI_PASS` | Senha da rede Wi-Fi. |
| `WOL_MAC_ADDRESS` | Endereço MAC da interface que receberá o Wake-on-LAN, no formato `AA:BB:CC:DD:EE:FF`. |
| `WOL_BROADCAST_IP` | Endereço de broadcast para envio do pacote WOL. |
| `WOL_PORT` | Porta UDP usada para Wake-on-LAN. Normalmente `9` ou `7`. |

## Comandos

O projeto inclui um `Makefile` com atalhos para o PlatformIO:

```sh
make build
```

Compila o firmware.

```sh
make upload
```

Compila e grava no ESP32.

```sh
make monitor
```

Abre o monitor serial em `115200` baud usando `/dev/ttyUSB0`.

```sh
make run
```

Faz upload e abre o monitor serial.

```sh
make reset
```

Executa o alvo customizado de reset e abre o monitor serial.

```sh
make clean
```

Remove artefatos de build do PlatformIO.

Para usar outra porta serial:

```sh
make monitor PORT=/dev/ttyACM0
make upload PORT=/dev/ttyACM0
```

Também é possível usar o PlatformIO diretamente:

```sh
pio run
pio run -t upload
pio device monitor -b 115200 -p /dev/ttyUSB0
```

## Estrutura

```text
.
├── src/main.cpp              # Fluxo principal do firmware
├── lib/api_client/           # Cliente HTTP para health check e shutdown
├── lib/schedule/             # Parser e cálculo da agenda de horas
├── lib/wifi/                 # Conexão Wi-Fi
├── lib/wol/                  # Montagem e envio do pacote Wake-on-LAN
├── scripts/prebuild.py       # Injeta variáveis do .env como macros C++
├── scripts/reset.py          # Alvo customizado de reset via esptool
├── platformio.ini            # Configuração do PlatformIO
└── Makefile                  # Atalhos de build, upload e monitor
```

## Observações

- O arquivo `.env` contém credenciais e não deve ser versionado.
- Se `HOURS` não tiver nenhuma hora válida, o firmware reinicia.
- O agendamento considera a hora local configurada por `TZ`.
- Quando o horário atual está dentro de uma hora agendada, o próximo despertar acontece no fim dessa hora.
- Fora da agenda, o próximo despertar acontece no início da próxima hora configurada.
