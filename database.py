import sqlite3
import json
from datetime import datetime

class Database:
    """
    Класс-одиночка (Singleton) для работы с SQLite.
    Обеспечивает единое подключение и логирование запросов.
    """
    _instance = None

    def __new__(cls, db_path='logs.db'):
        if cls._instance is None:
            cls._instance = super(Database, cls).__new__(cls)
            cls._instance._initialize(db_path)
        return cls._instance

    def _initialize(self, db_path):
        """Приватный метод: создаёт таблицу, если её нет."""
        self.db_path = db_path
        self.conn = sqlite3.connect(db_path, check_same_thread=False)
        self.cursor = self.conn.cursor()
        self._create_table()

    def _create_table(self):
        """Создаёт таблицу api_logs, если она отсутствует."""
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS api_logs (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                endpoint TEXT NOT NULL,
                params TEXT,
                result TEXT,
                timestamp TEXT NOT NULL
            )
        ''')
        self.conn.commit()

    def log_request(self, endpoint: str, params: dict, result: dict):
        """
        Сохраняет информацию о запросе в БД.
        params и result преобразуются в JSON-строки.
        """
        params_json = json.dumps(params, ensure_ascii=False)
        result_json = json.dumps(result, ensure_ascii=False)
        timestamp = datetime.now().isoformat()
        self.cursor.execute('''
            INSERT INTO api_logs (endpoint, params, result, timestamp)
            VALUES (?, ?, ?, ?)
        ''', (endpoint, params_json, result_json, timestamp))
        self.conn.commit()

    def get_all_logs(self, limit=100):
        """Возвращает последние записи (опционально, для отладки)."""
        self.cursor.execute('''
            SELECT id, endpoint, params, result, timestamp
            FROM api_logs
            ORDER BY id DESC LIMIT ?
        ''', (limit,))
        rows = self.cursor.fetchall()
        return [
            {
                'id': r[0],
                'endpoint': r[1],
                'params': json.loads(r[2]),
                'result': json.loads(r[3]),
                'timestamp': r[4]
            }
            for r in rows
        ]

    def close(self):
        """Закрывает соединение с БД (вызывать при завершении работы)."""
        if self.conn:
            self.conn.close()