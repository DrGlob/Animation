#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <unistd.h>
#include <random>
#include <algorithm>

const int width = 80;
const int height = 30;

const std::string dark_gray = "\033[90m";
const std::string light_gray = "\033[37m";
const std::string red = "\033[91m";
const std::string green = "\033[92m";
const std::string reset = "\033[0m";

struct Fighter {
    double x, y;
    double speed;
    bool active;
    bool is_xwing;
    int health;
};

struct Laser {
    double x, y;
    double speed;
    bool active;
};

std::vector<Fighter> fighters;
std::vector<Laser> lasers;
std::random_device rd;
std::mt19937 gen(rd());

void spawnFighter() {
    static double spawn_timer = 0;
    spawn_timer += 0.1;
    
    if (spawn_timer > 3.0 && fighters.size() < 5) {
        Fighter fighter;
        fighter.x = width + 5;
        fighter.y = std::uniform_real_distribution<double>(5, height - 5)(gen);
        fighter.speed = std::uniform_real_distribution<double>(0.3, 0.8)(gen);
        fighter.active = true;
        fighter.is_xwing = std::uniform_real_distribution<double>(0, 1)(gen) > 0.3;
        fighter.health = 3;
        fighters.push_back(fighter);
        spawn_timer = 0;
    }
}

void updateFighters() {
    for (auto& fighter : fighters) {
        if (fighter.active) {
            fighter.x -= fighter.speed;
            
            // Периодически стреляют
            if (std::uniform_real_distribution<double>(0, 1)(gen) < 0.02) {
                Laser laser;
                laser.x = fighter.x - 1;
                laser.y = fighter.y;
                laser.speed = 2.0;
                laser.active = true;
                lasers.push_back(laser);
            }
            
            // Удаляем вылетевших за экран
            if (fighter.x < -5) {
                fighter.active = false;
            }
        }
    }
    
    // Удаляем неактивных истребителей
    fighters.erase(std::remove_if(fighters.begin(), fighters.end(), 
        [](const Fighter& f) { return !f.active; }), fighters.end());
}

void updateLasers() {
    for (auto& laser : lasers) {
        if (laser.active) {
            laser.x -= laser.speed; // Все лазеры летят только слева направо (от истребителей)
            
            // Удаляем вылетевшие за экран лазеры
            if (laser.x < -5) {
                laser.active = false;
            }
        }
    }
    
    // Удаляем неактивные лазеры
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(), 
        [](const Laser& l) { return !l.active; }), lasers.end());
}

void renderDeathStar() {
    std::vector<std::vector<char>> screen(height, std::vector<char>(width, ' '));
    std::vector<std::vector<std::string>> colors(height, std::vector<std::string>(width, reset));
    
    static double angle = 0;
    angle += 0.05;
    
    // Рендер Звезды Смерти
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double x = (j - width/2.0) / (width/4.0);
            double y = (i - height/2.0) / (height/3.0);
            
            double rx = x * cos(angle) - y * sin(angle);
            double ry = x * sin(angle) + y * cos(angle);
            
            double dist = rx*rx + ry*ry;
            
            if (dist < 1.0) {
                double crater_x = rx - 0.4;
                double crater_y = ry - 0.1;
                double crater_dist = crater_x*crater_x + crater_y*crater_y;
                
                if (crater_dist < 0.1) {
                    if (crater_dist < 0.02) {
                        screen[i][j] = 'O';
                        colors[i][j] = red; // Изменено с blue на red
                    } else {
                        screen[i][j] = '0';
                        colors[i][j] = light_gray;
                    }
                } else {
                    if (fmod(j + i * 2 + angle * 50, 6) < 2) {
                        screen[i][j] = '#';
                        colors[i][j] = dark_gray;
                    } else {
                        screen[i][j] = '.';
                        colors[i][j] = light_gray;
                    }
                }
            }
        }
    }
    
    // Рендер истребителей
    for (const auto& fighter : fighters) {
        if (fighter.active) {
            int x = static_cast<int>(fighter.x);
            int y = static_cast<int>(fighter.y);
            
            if (x >= 0 && x < width && y >= 0 && y < height) {
                if (fighter.is_xwing) {
                    screen[y][x] = 'X';
                    colors[y][x] = green;
                    // Крылья X-wing
                    if (x+1 < width) { screen[y][x+1] = '-'; colors[y][x+1] = green; }
                    if (x-1 >= 0) { screen[y][x-1] = '-'; colors[y][x-1] = green; }
                    if (y-1 >= 0) { screen[y-1][x] = '|'; colors[y-1][x] = green; }
                    if (y+1 < height) { screen[y+1][x] = '|'; colors[y+1][x] = green; }
                } else {
                    screen[y][x] = 'T';
                    colors[y][x] = red;
                    // Крылья TIE fighter
                    if (x+1 < width) { screen[y][x+1] = '='; colors[y][x+1] = red; }
                    if (x-1 >= 0) { screen[y][x-1] = '='; colors[y][x-1] = red; }
                }
            }
        }
    }
    
    // Рендер лазеров (только красные - от истребителей)
    for (const auto& laser : lasers) {
        if (laser.active) {
            int x = static_cast<int>(laser.x);
            int y = static_cast<int>(laser.y);
            
            if (x >= 0 && x < width && y >= 0 && y < height) {
                screen[y][x] = '<'; // Лазеры летят к Звезде Смерти
                colors[y][x] = red;
            }
        }
    }
    
    // Вывод на экран
    std::cout << "\x1b[H";
    std::cout << "ЗВЕЗДА СМЕРТИ - Орбитальная боевая станция | Истребители: " << fighters.size() << "\n";
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << colors[i][j] << screen[i][j];
        }
        std::cout << reset << '\n';
    }
    
    // Статус бар
    std::cout << "Лазеры: " << lasers.size() << " | ";
    std::cout << "Атакуют Звезду Смерти!\n";
    std::cout << std::flush;
}

int main() {
    std::cout << "\x1b[2J\x1b[?25l" << std::flush;
    
    try {
        while (true) {
            spawnFighter();
            updateFighters();
            updateLasers();
            renderDeathStar();
            usleep(100000);
        }
    } catch (...) {
        std::cout << "\x1b[?25h\n";
    }
    
    return 0;
}