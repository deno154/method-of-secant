# Демонстрация работы проекта.

---

## Сборка.

**macOS**:

```bash
cd /Users/deno/MOS/method-of-secant/
rm -rf build
mkdir build
cd build

cmake ..
make
./server
```

---

**windows 10**:

```bash
mkdir build
cd build

cmake ..
mingw32-make

server.exe
```

---

## Демонстрация работы сервера и функций.

Проверка неизвестного endpoint:
```bash
curl http://localhost:8080/test
```

Ожидаемый вывод: 
```json
{"status":"error","message":"Unknown endpoint"}
```

---

Проверка /md5:
```bash
curl -X POST http://localhost:8080/md5 \
-H "Content-Type: application/json" \
-d "{\"text\":\"hello\"}"
```

Ожидаемый вывод:
```json
{
    "status":"ok",
    "algorithm":"md5",
    "message":"stub"
}
```

Проверяет:
- POST запрос
- обработку JSON
- endpoint /md5
- вызов handler
- корректный JSON response

---

Проверка /vigenere:
```bash
curl -X POST http://localhost:8080/vigenere \
-H "Content-Type: application/json" \
-d "{\"text\":\"hello\",\"key\":\"abc\"}"
```

Ожидаемый вывод:
```json
{
    "status":"ok",
    "algorithm":"vigenere",
    "message":"stub"
}
```

Проверяет:
- endpoint /vigenere
- JSON body parsing
- обработчик шифра Виженера

---

Проверка /secant:
```bash
curl -X POST http://localhost:8080/secant \
-H "Content-Type: application/json" \
-d "{\"a\":\"1\",\"b\":\"2\"}"
```

Ожидаемый вывод:
```json
{
    "status":"ok",
    "algorithm":"secant",
    "message":"stub"
}
```

Проверяет:
- endpoint метода секущих
- обработку параметров
- вызов математического handler

---

Проверка /graph-cycle:
```bash
curl -X POST http://localhost:8080/graph-cycle \
-H "Content-Type: application/json" \
-d "{\"graph\":\"1-2,2-3,3-1\"}"
```

Ожидаемый вывод:
```json
{
    "status":"ok",
    "algorithm":"graph_cycle",
    "message":"stub"
}
```
Проверяет:
- endpoint графов
- обработку JSON
- вызов алгоритма проверки цикла

---

## Проверка работы HTTP сервера после подключения БД.

```bash
curl -X POST http://localhost:8080/md5 \
-H "Content-Type: application/json" \
-d "{\"text\":\"hello\"}"
```

Ожидаемый вывод:
```bash
{
    "status":"ok",
    "algorithm":"md5",
    "message":"stub"
}
```

Проверяет:
- сервер продолжает работать после подключения БД
- singleton БД не ломает HTTP сервер
- routing и handlers работают корректно

---
