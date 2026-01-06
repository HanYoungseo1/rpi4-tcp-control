# Raspberry Pi Device Control Project

이 프로젝트는 Raspberry Pi에서 동작하는 다양한 장치(LED, Buzzer, Sensor, segment 등)를 TCP 소켓을 통해 원격 제어하는 서버/클라이언트 시스템입니다.

서버는 Raspberry Pi에서 데몬 프로세스로 실행되며,
클라이언트는 Linux Ubuntu 환경에서 서버에 접속하여 문자열 명령을 통해 장치를 제어합니다.

---

## Project Overview

- Server: Raspberry Pi 4 (ARM64)

- Client: Ubuntu Linux

- Communication: TCP/IP Socket

- Concurrency: Multi-threaded device control

- Modularity: Shared library (.so) based device drivers

- Build: Cross-compilation + Makefile automation

---

## Supported Devices & Behavior

### LED

- 클라이언트 명령을 통해 ON / OFF 제어

- 밝기 단계 조절 (최대 / 중간 / 최소)

- Sensor와 연동 시 자동 제어


### Buzzer

- 클라이언트 명령으로 소리 ON / OFF 제어

- 7-Segment 카운트다운 종료 시 알림용으로 동작


### Light Sensor

- 클라이언트에서 조도 센서 값 확인

- 빛이 감지되지 않으면 LED On, 빛이 감지되면 LED Off


### Temperature Sensor

- 클라이언트에서 온도 센서 값 확인

- 온도가 설정한 값보다 높으면 LED On, 낮으면 LED Off


### 7-Segment Display

- 클라이언트에서 전송한 숫자(0~9) 표시 (초 단위 시간)

- 1초 지날 때마다 -1씩 감소 표시

- 0이 되면 부저 울림

---

## System Architecture

- 서버는 TCP 연결을 유지하며 클라이언트 명령을 수신

- 각 장치 제어는 기능별 스레드로 분리되어 실행

- 실제 하드웨어 제어 로직은 공유 라이브러리(.so) 형태로 구현

- 서버는 런타임에 dlopen() / dlsym()을 통해 장치 제어 로직을 로딩

- 서버는 데몬 프로세스로 실행되어 터미널 종료와 무관하게 동작

---

## Build & Run

### 1. Makefile 내 IP 수정

```bash
IP = 192.168.0.49 
```

Makefile 내 IP 변수의 값을
클라이언트가 연결할 서버의 IP 주소로 수정하세요.


### 2. 실행 파일 및 라이브러리 빌드

```bash
./exec.sh
```

이 스크립트는 다음을 수행합니다.

- 서버 실행 파일 `build/server` 생성
- 클라이언트 실행 파일 `build/client` 생성
- 공유 라이브러리 (`lib_led.so`, `lib_buzzer.so` 등) 빌드


### 3. 서버 실행 (라즈베리 파이 등 장치 측)

```bash
./build/server
```

서버는 포트 `60000`에서 클라이언트 연결을 기다립니다. 장치 제어는 각 메시지에 따라 쓰레드로 동작합니다.


### 4. 클라이언트 실행 (Ubuntu 등 제어 측)

```bash
./build/client
```

클라이언트는 TCP를 통해 서버에 연결하고, 장치 제어 명령을 보낼 수 있는 인터페이스를 제공합니다.

---

## Project Structure

```
project/
├── build/               # 실행 파일 및 라이브러리 출력 폴더
├── server/
│   ├── main.c
│   ├── device/          # 쓰레드 기반 장치 제어 코드
│   └── libs/            # 공유 라이브러리 코드 (lib_xxx.so)
├── client/              # 클라이언트 소스
├── Makefile
└── exec.sh              # 빌드 자동화 스크립트
```

---

## GPIO Pin Configuration (BCM)

- LED → GPIO 17

- Buzzer → GPIO 25

- 7-Segment

    A: GPIO 14

    B: GPIO 15

    C: GPIO 18

    D: GPIO 23

---

## Requirements

- `wiringPi` 라이브러리 (크로스 컴파일 환경에서도 설치 필요)

- `aarch64-linux-gnu-gcc` (ARM 타겟용 컴파일러)
