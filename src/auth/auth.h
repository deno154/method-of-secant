#ifndef AUTH_H
#define AUTH_H

#include <string>
/**
 * @brief Регистрация пользователя.
 */
std::string registerUser(const std::string &request);
/**
 * @brief Авторизация пользователя.
 */
std::string loginUser(const std::string &request);

#endif