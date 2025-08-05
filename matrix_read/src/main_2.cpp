#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <cmath>
#include "exprtk.hpp"

std::vector<std::vector<double>> avaliar_matriz(
    const std::string& file_name,
    size_t linhas,
    size_t colunas,
    const std::unordered_map<std::string, double>& vars)
{
    using symbol_table_t = exprtk::symbol_table<double>;
    using expression_t = exprtk::expression<double>;
    using parser_t = exprtk::parser<double>;

    std::ifstream arq(file_name);
    if (!arq.is_open()) {
        throw std::runtime_error("Error while trying to open the file: " + file_name);
    }

    std::vector<std::vector<double>> matriz(linhas, std::vector<double>(colunas));

    std::vector<double> valores_vars;
    std::vector<std::string> nomes_vars;

    for (const auto& [nome, valor] : vars) {
        valores_vars.push_back(valor);
        nomes_vars.push_back(nome);
    }

    symbol_table_t sym_table;
    for (size_t i = 0; i < nomes_vars.size(); ++i) {
        sym_table.add_variable(nomes_vars[i], valores_vars[i]);
    }
    sym_table.add_constants();

    exprtk::parser<double> parser;

    for (size_t i = 0; i < linhas; ++i) {
        for (size_t j = 0; j < colunas; ++j) {
            std::string linha;
            if (!std::getline(arq, linha)) {
                throw std::runtime_error("File doesn't contain enough expressions.");
            }

            expression_t expr;
            expr.register_symbol_table(sym_table);

            if (!parser.compile(linha, expr)) {
                throw std::runtime_error("Expression Error: " + linha + "\nErro: " + parser.error());
            }

            matriz[i][j] = expr.value();
        }
    }

    return matriz;
}

double random_double(double min, double max, std::mt19937& rng) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int i = 1; i <= 3; ++i) {
        std::unordered_map<std::string, double> vars;

        // Gera valores aleatórios realistas
        vars["L0"] = random_double(0.5, 2.0, rng);
        vars["L1"] = random_double(0.5, 2.0, rng);
        vars["s"] = random_double(0.5, 2.0, rng);
        vars["theta0"] = random_double(0.0, 3.14, rng); // from 0 to pi
        vars["phi0"] = random_double(0.0, 3.14, rng);
        vars["theta1"] = random_double(0.0, 3.14, rng);
        vars["phi1"] = random_double(0.0, 3.14, rng);

        // Calcula variáveis derivadas
        vars["ss1"] = sin(vars["s"] * vars["theta1"]);
        vars["cp0"] = cos(vars["phi0"]);
        vars["sp0"] = sin(vars["phi0"]);
        vars["st0"] = sin(vars["theta0"]);
        vars["cst11"] = cos(vars["s"] * vars["theta1"]) - 1;
        vars["cp1"] = cos(vars["phi1"]);
        vars["sp1"] = sin(vars["phi1"]);
        vars["ct0"] = cos(vars["theta0"]);
        vars["sp0pow"] = pow(vars["sp0"], 2);
        vars["cp0pow"] = pow(vars["cp0"], 2);

        std::cout << "\n=== Random Combination " << i << " ===\n";
        for (const auto& [k, v] : vars) {
            std::cout << k << " = " << v << "\n";
        }

        try {
            auto matriz = avaliar_matriz("fk_translation.txt", 3, 1, vars);
            std::cout << "Calculated matrix:\n";
            for (const auto& linha : matriz) {
                for (auto val : linha) {
                    std::cout << val << " ";
                }
                std::cout << "\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error while trying to calculate matrix: " << e.what() << "\n";
        }
    }

    return 0;
}

