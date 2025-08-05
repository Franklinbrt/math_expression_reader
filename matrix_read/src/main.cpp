#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "exprtk.hpp"

std::vector<std::vector<double>> avaliar_matriz(
    const std::string& nome_arquivo,
    size_t linhas,
    size_t colunas,
    const std::unordered_map<std::string, double>& vars)
{
    using symbol_table_t = exprtk::symbol_table<double>;
    using expression_t = exprtk::expression<double>;
    using parser_t = exprtk::parser<double>;

    std::ifstream arq(nome_arquivo);
    if (!arq.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo: " + nome_arquivo);
    }

    std::vector<std::vector<double>> matriz(linhas, std::vector<double>(colunas));

    // Guardar valores variáveis para manter referência válida
    std::vector<double> valores_vars;
    std::vector<std::string> nomes_vars;

    valores_vars.reserve(vars.size());
    nomes_vars.reserve(vars.size());

    for (const auto& [nome, valor] : vars) {
        valores_vars.push_back(valor);
        nomes_vars.push_back(nome);
    }

    symbol_table_t sym_table;
    for (size_t i = 0; i < nomes_vars.size(); ++i) {
        sym_table.add_variable(nomes_vars[i], valores_vars[i]);
    }
    sym_table.add_constants();

    parser_t parser;

    for (size_t i = 0; i < linhas; ++i) {
        for (size_t j = 0; j < colunas; ++j) {
            std::string linha;
            if (!std::getline(arq, linha)) {
                throw std::runtime_error("Arquivo não contém expressões suficientes.");
            }

            expression_t expr;
            expr.register_symbol_table(sym_table);

            if (!parser.compile(linha, expr)) {
                throw std::runtime_error("Erro na expressão: " + linha + "\nErro: " + parser.error());
            }

            matriz[i][j] = expr.value();
        }
    }

    return matriz;
}

int main() {
    std::unordered_map<std::string, double> vars = {
        {"theta0", 1.0}, {"theta1", 1.2}, {"phi0", 0.5}, {"phi1", 0.6},
        {"L0", 200.0}, {"L1", 100.5}, {"s", 1.0}
    };

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

    try {
        auto matriz = avaliar_matriz("fk_translation.txt", 3, 1, vars);

        std::cout << "Matriz calculada:\n";
        for (const auto& linha : matriz) {
            for (auto val : linha) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}

