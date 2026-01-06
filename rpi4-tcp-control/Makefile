CC = aarch64-linux-gnu-gcc
CFLAGS = -I/usr/aarch64-linux-gnu/include -Wall -fPIC
LDFLAGS = -L/usr/aarch64-linux-gnu/lib -Lbuild/libs -lwiringPi -lpthread -lcrypt

# 디렉토리 구조
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/libs
TARGET = $(BUILD_DIR)/server
IP = 192.168.0.49
# 소스 및 오브젝트 정의
SRCS = \
	server/main.c \
	server/device/thread_led.c \
	server/device/thread_buzzer.c \
	server/device/thread_sensor.c \
	server/device/thread_segment.c \
	server/device/thread_temperature.c

OBJS = $(SRCS:.c=.o)

# lib_x.so → build/libs/lib_x.so
LIB_TARGETS = \
    $(LIB_DIR)/lib_sensor.so \
    $(LIB_DIR)/lib_led.so \
    $(LIB_DIR)/lib_buzzer.so \
    $(LIB_DIR)/lib_segment.so \
	$(LIB_DIR)/lib_temperature.so

# 기본 타겟
all: $(TARGET)

# 빌드 디렉토리 생성
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR): | $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

# 실행 파일 빌드
$(TARGET): $(OBJS) $(LIB_TARGETS) | $(BUILD_DIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	scp -r $(BUILD_DIR) veda@$(IP):/home/veda

# 오브젝트 빌드 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 공유 라이브러리 빌드 규칙
$(LIB_DIR)/lib_sensor.so: server/libs/sensor_control.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $< -lwiringPi

$(LIB_DIR)/lib_led.so: server/libs/led_control.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $< -lwiringPi

$(LIB_DIR)/lib_buzzer.so: server/libs/buzzer_control.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $< -lwiringPi

$(LIB_DIR)/lib_segment.so: server/libs/segment_control.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $< -lwiringPi

$(LIB_DIR)/lib_temperature.so: server/libs/temperature_control.c | $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $< -lwiringPi

# 청소
clean:
	rm -f server/*.o server/device/*.o
	rm -rf $(BUILD_DIR)
