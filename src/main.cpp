#include "benchmarks/ref-benchmark.h"
#include "benchmarks/ecs-benchmark.h"
#include "benchmarks/oop-benchmark.h"

int main() {
    ecs_benchmark application;
    return application.run();
}