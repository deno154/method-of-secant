import hashlib
import math
from flask import Flask, request, jsonify
from database import Database

app = Flask(__name__)
db = Database()

# ===================== 1. Шифр Виженера =====================
def vigenere_cipher(text: str, key: str, mode: str = "encrypt") -> str:
    """
    Реальное шифрование/дешифрование Виженера для A-Z.
    mode = 'encrypt' или 'decrypt'.
    Неалфавитные символы остаются без изменений.
    """
    if not key:
        return text  # нет ключа — без изменений

    # Приводим к верхнему регистру и оставляем только буквы A-Z для ключа
    key_clean = ''.join(ch for ch in key.upper() if ch.isalpha())
    if not key_clean:
        return text

    result = []
    key_len = len(key_clean)
    key_index = 0

    for ch in text:
        if ch.isalpha():
            # Определяем регистр исходной буквы
            is_upper = ch.isupper()
            base = ord('A')
            # Приводим к числу 0-25
            ch_num = ord(ch.upper()) - base

            # Сдвиг из ключа
            shift = ord(key_clean[key_index % key_len]) - base
            if mode == 'encrypt':
                new_num = (ch_num + shift) % 26
            else:  # decrypt
                new_num = (ch_num - shift) % 26

            new_char = chr(new_num + base)
            # Восстанавливаем регистр
            if not is_upper:
                new_char = new_char.lower()
            result.append(new_char)
            key_index += 1
        else:
            # Не буквы (пробелы, знаки препинания) не шифруем
            result.append(ch)
    return ''.join(result)


# ===================== 2. MD5 хэш =====================
def md5_hash(data: str) -> str:
    """Возвращает MD5 хэш строки в виде 32-символьной шестнадцатеричной строки."""
    return hashlib.md5(data.encode('utf-8')).hexdigest()


# ===================== 3. Метод секущих =====================
def secant_method(func_expr: str, x0: float, x1: float, tol: float = 1e-6, max_iter: int = 100) -> dict:
    """
    Находит корень уравнения f(x)=0 методом секущих.
    func_expr — строка с выражением на Python, использующая 'x' как переменную.
    Пример: "x**2 - 2"  (корень sqrt(2)).
    Возвращает словарь: {'root': корень, 'iterations': кол-во итераций, 'message': статус}
    """
    # Определяем функцию f(x) безопасно через eval (ограничим пространство имён)
    allowed_names = {
        'x': 0,
        'abs': abs,
        'pow': pow,
        'math': math,
        'sin': math.sin,
        'cos': math.cos,
        'tan': math.tan,
        'exp': math.exp,
        'log': math.log,
        'sqrt': math.sqrt,
        'pi': math.pi,
        'e': math.e
    }
    # Дополнительно разрешим функции из math
    try:
        # Проверяем, что выражение безопасно (не содержит __import__ и т.п.)
        code = compile(func_expr, '<string>', 'eval')
        for name in code.co_names:
            if name not in allowed_names and name != 'x':
                return {'root': None, 'iterations': 0, 'message': f'Недопустимая функция: {name}'}
    except Exception as e:
        return {'root': None, 'iterations': 0, 'message': f'Ошибка в выражении: {str(e)}'}

    def f(x):
        return eval(func_expr, {"__builtins__": {}}, {**allowed_names, 'x': x})

    # Метод секущих
    x_prev = x0
    x_curr = x1
    f_prev = f(x_prev)
    for i in range(max_iter):
        f_curr = f(x_curr)
        if abs(f_curr) < tol:
            return {'root': x_curr, 'iterations': i+1, 'message': 'Найден корень'}
        if f_curr - f_prev == 0:
            return {'root': None, 'iterations': i+1, 'message': 'Разность функций нулевая, деление на ноль'}
        # Вычисляем следующее приближение
        x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev)
        # Проверка сходимости (изменение меньше tol)
        if abs(x_next - x_curr) < tol:
            return {'root': x_next, 'iterations': i+2, 'message': 'Корень найден по изменению аргумента'}
        x_prev, x_curr = x_curr, x_next
        f_prev = f_curr

    return {'root': x_curr, 'iterations': max_iter, 'message': 'Не сошлось за максимальное число итераций'}


# ===================== 4. Проверка цикла в графе =====================
def check_graph_cycle(adjacency_list: dict, user_answer: list) -> bool:
    """
    Проверяет, что user_answer (список вершин) является простым циклом в графе.
    - Все вершины должны существовать в adj_list.
    - Каждая пара соседних вершин (включая последнюю с первой) должна быть связана ребром.
    - Внутри цикла не должно быть повторяющихся вершин (кроме повторения первой в конце).
    - Цикл должен содержать не менее 3 вершин (простой цикл).
    """
    if not isinstance(user_answer, list) or len(user_answer) < 3:
        return False

    # Проверяем, что все вершины есть в графе
    for v in user_answer:
        if v not in adjacency_list:
            return False

    # Проверяем рёбра между последовательными вершинами
    n = len(user_answer)
    for i in range(n - 1):
        u = user_answer[i]
        v = user_answer[i+1]
        # Ребро должно быть в списке смежности (ориентированный или неориентированный? 
        # Будем считать граф неориентированным — проверяем оба направления)
        if not (v in adjacency_list.get(u, []) or u in adjacency_list.get(v, [])):
            return False
    # Замыкающее ребро (последняя -> первая)
    first, last = user_answer[0], user_answer[-1]
    if not (last in adjacency_list.get(first, []) or first in adjacency_list.get(last, [])):
        return False

    # Проверка на повторение вершин внутри (кроме первой и последней)
    # Если первая и последняя совпадают — вычтем, но по логике цикла они равны,
    # но в user_answer обычно они разные (первая и последняя — граничные).
    # Для простоты считаем, что user_answer задаёт последовательность, где первая и последняя 
    # могут быть разными (тогда цикл замыкается ребром между ними).
    # Повторяющихся вершин не должно быть.
    seen = set()
    for v in user_answer:
        if v in seen:
            return False
        seen.add(v)
    # Если первая и последняя не совпадают, то условие выше не сработает — это ок.
    # Однако, если список содержит одну вершину дважды в середине, это обнаружит.
    return True


# ===================== Обработчики запросов =====================
@app.route('/')
def index():
    return jsonify({
        "status": "ok",
        "endpoints": {
            "/vigenere": "POST with json: {text, key, mode}",
            "/md5": "POST with json: {data}",
            "/secant": "POST with json: {func_expr, x0, x1, tol?}",
            "/graph_cycle": "POST with json: {graph, user_answer}"
        }
    })

@app.route('/vigenere', methods=['POST'])
def handle_vigenere():
    data = request.get_json()
    if not data:
        return jsonify({"error": "missing JSON"}), 400
    text = data.get('text', '')
    key = data.get('key', '')
    mode = data.get('mode', 'encrypt')
    result = vigenere_cipher(text, key, mode)
    return jsonify({"result": result})

@app.route('/md5', methods=['POST'])
def handle_md5():
    data = request.get_json()
    if not data:
        return jsonify({"error": "missing JSON"}), 400
    text = data.get('data', '')
    result = md5_hash(text)
    return jsonify({"result": result})

@app.route('/secant', methods=['POST'])
def handle_secant():
    data = request.get_json()
    if not data:
        return jsonify({"error": "missing JSON"}), 400
    func_expr = data.get('func_expr', '')
    x0 = float(data.get('x0', 0.0))
    x1 = float(data.get('x1', 1.0))
    tol = float(data.get('tol', 1e-6))
    result = secant_method(func_expr, x0, x1, tol)
    return jsonify(result)

@app.route('/graph_cycle', methods=['POST'])
def handle_graph_cycle():
    data = request.get_json()
    if not data:
        return jsonify({"error": "missing JSON"}), 400
    graph = data.get('graph', {})
    user_answer = data.get('user_answer', [])
    is_cycle = check_graph_cycle(graph, user_answer)
    return jsonify({"is_cycle": is_cycle})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)