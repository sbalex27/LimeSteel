// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <vector>

// add headers that you want to pre-compile here
constexpr auto TEST_DIRECTORY = "C:\\Users\\alex2\\LimeSteel\\Tests";

// Función de ayuda para comparar dos vectores
template<typename T>
bool vector_equals(const std::vector<T>& vec1, const std::vector<T>& vec2) {
	if (vec1.size() != vec2.size()) {
		return false; // Si los vectores tienen tamaños diferentes, no son iguales
	}

	// Compara los elementos de los vectores uno a uno
	for (size_t i = 0; i < vec1.size(); ++i) {
		if (vec1[i] != vec2[i]) {
			return false; // Si un elemento no coincide, los vectores no son iguales
		}
	}

	return true; // Si todos los elementos coinciden, los vectores son iguales
}

#endif //PCH_H
