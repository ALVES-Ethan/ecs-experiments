#include "benchmarks/oop-benchmark.h"
#include "benchmarks/ecs-benchmark.h"

int main() {
    ecs_benchmark application;
    return application.run();
}