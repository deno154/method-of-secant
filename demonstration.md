# Демонстрация работы проекта.

Логи сервера выводятся в терминал, в котором запущено серверное приложение. HTTP-запросы отображаются через стандартный вывод (std::cout) в момент обработки запроса. Ответы сервера возвращаются в терминал клиента (curl), где отображается результат выполнения запроса в формате JSON.

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

![start](screenshoots_demo/start.jpg)

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
![endpoint](screenshoots_demo/endpoint.jpg)

![endpoint_q](screenshoots_demo/endpoint_q.jpg)

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

![functional](screenshoots_demo/functional.jpg)

![functional_q](screenshoots_demo/functional.jpg)

![functional_w](screenshoots_demo/functional.jpg)


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

HTTP POST-запросы имеют одинаковую структуру (Content-Type: application/json), различаясь только маршрутом (endpoint), который определяет вызываемый обработчик сервера.

![connected_db](screenshoots_demo/connected_db.jpg)

![connected_db_q](screenshoots_demo/connected_db_q.jpg)

---

## Проверка многопоточной обработки HTTP-запросов.

MD5 запрос:
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

Vigenere запрос:
```bash
curl -X POST http://localhost:8080/vigenere \
-H "Content-Type: application/json" \
-d "{\"text\":\"hello\",\"key\":\"abc\"}"
```

Ожидаемый вывод:
```bash
{
    "status":"ok",
    "algorithm":"vigenere",
    "message":"stub"
}
```

Проверяет:
- сервер обрабатывает несколько HTTP-запросов подряд
- каждый запрос выполняется в отдельном потоке (multi-client support)
- routing работает корректно для разных endpoint’ов
- обработчики (handlers) вызываются независимо друг от друга
- сервер не блокируется при параллельных запросах

Из-за высокой скорости обработки запросов параллельность выполнения не всегда визуально отражается в логах.

![client](screenshoots_demo/client.jpg)

![client_q](screenshoots_demo/client_q.jpg)

---

