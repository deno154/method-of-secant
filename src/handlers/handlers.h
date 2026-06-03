#ifndef HANDLERS_H
#define HANDLERS_H

#include <string>
/**
 * @brief Обработчик шифра Виженера.
 */
std::string handleVigenere(const std::string &body);
/**
 * @brief Обработчик алгоритма MD5.
 */
std::string handleMD5(const std::string &body);
/**
 * @brief Обработчик метода секущих.
 */
std::string handleSecant(const std::string &body);

/**
 * @brief Обработчик проверки цикла в графе.
 */
std::string handleGraphCycle(const std::string &body);

#endif