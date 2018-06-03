#define BOOST_TEST_MODULE VehicleTests

#include <boost/test/included/unit_test.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "Vehicle.h"
#include "World.h"
#include "Application.h"
#include "EvolutionaryAlgorithm.h"
#include <algorithm>

BOOST_AUTO_TEST_SUITE(vehicle_tests_suite)

    BOOST_AUTO_TEST_CASE(check_if_deleteBody_deletes_b2Body) {

        int count = World::getInstance().getBodyCount();
        BOOST_CHECK_EQUAL(count, 0);
        {
            EvolutionaryAlgorithm algo(20, 2, 0.1f);
            std::vector<Vehicle> vehicles(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
            for (auto& v : vehicles)
                v.deleteBody();
        }
        BOOST_CHECK_EQUAL(World::getInstance().getBodyCount(), 0);
    }

    BOOST_AUTO_TEST_CASE(check_if_chromosome_and_vehicle_have_same_number_of_wheels) {
        for (size_t i = 0; i < Chromosome::VERTICES_NUMBER; ++i) {
            EvolutionaryAlgorithm algo(20, i, 0.1f);
            std::vector<Chromosome> chromosomes(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
            std::vector<Vehicle> vehicles(chromosomes.begin(), chromosomes.end());
            for (size_t j = 0; j < vehicles.size(); ++j)
                BOOST_CHECK_EQUAL(chromosomes[j].WHEELS_NUMBER, vehicles[j].getWheelShapes().size());
            for (auto& v : vehicles)
                v.deleteBody();
        }

    }

    BOOST_AUTO_TEST_CASE(check_if_chromosome_and_vehicle_have_same_number_of_vertices) {
        EvolutionaryAlgorithm algo(20, 2, 0.1f);
        std::vector<Chromosome> chromosomes(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
        std::vector<Vehicle> vehicles(chromosomes.begin(), chromosomes.end());
        for (size_t i = 0; i < vehicles.size(); ++i)
            BOOST_CHECK_EQUAL(chromosomes[i].VERTICES_NUMBER, vehicles[i].getChassisShape().getPointCount());
        for (auto& v : vehicles)
            v.deleteBody();
    }

    BOOST_AUTO_TEST_CASE(check_if_vertices_in_chromosome_and_vehicle_are_equal) {
        EvolutionaryAlgorithm algo(20, 2, 0.1f);
        std::vector<Chromosome> chromosomes(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
        std::vector<Vehicle> vehicles(chromosomes.begin(), chromosomes.end());

        auto chromosome = chromosomes.begin();
        auto vehicle = vehicles.begin();
        for (; chromosome != chromosomes.end(); ++chromosome, ++vehicle) {

            std::vector<std::pair<float, float>> chassisVertices;
            for (size_t i = 0; i < vehicle->getChassisShape().getPointCount(); ++i) {
                chassisVertices.emplace_back(
                        vehicle->getChassisShape().getPoint(i).x / SCALE,
                        vehicle->getChassisShape().getPoint(i).y / SCALE);
            }

            BOOST_CHECK_EQUAL(std::is_permutation(chassisVertices.begin(),
                                                  chassisVertices.end(),
                                                  chromosome->getBodyVertices().begin(),
                                                  [](const std::pair<float, float>& a,
                                                     const std::pair<float, float>& b) -> bool {

                                                      return std::abs(a.first - b.first) < 0.01 &&
                                                             std::abs(a.second - b.second) < 0.01;
                                                  }), true);
        }
        for (auto& v : vehicles)
            v.deleteBody();
    }

    BOOST_AUTO_TEST_CASE(check_if_wheel_sizes_in_chromosome_and_vehicle_are_equal) {
        EvolutionaryAlgorithm algo(20, 2, 0.1f);
        std::vector<Chromosome> chromosomes(algo.GetCurrentGeneration().begin(), algo.GetCurrentGeneration().end());
        std::vector<Vehicle> vehicles(chromosomes.begin(), chromosomes.end());
        auto chromosome = chromosomes.begin();
        auto vehicle = vehicles.begin();
        for (; chromosome != chromosomes.end(); ++chromosome, ++vehicle) {
            std::vector<float> wheels;
            for (auto& wheel : vehicle->getWheelShapes())
                wheels.emplace_back(wheel.getRadius() / SCALE);

            BOOST_CHECK_EQUAL(std::is_permutation(chromosome->getWheelSizes().begin(),
                                                  chromosome->getWheelSizes().end(),
                                                  wheels.begin(),
                                                  [](const float& a,
                                                     const float& b) -> bool {

                                                      return std::abs(a - b) < 0.01;
                                                  }), true);
        }
        for (auto& v : vehicles)
            v.deleteBody();


    }

    BOOST_AUTO_TEST_CASE(check_if_world_deletes_body) {
        int bodyCounter = World::getInstance().getBodyCount();
        BOOST_CHECK_EQUAL(0, bodyCounter);
        auto dynamicBody = World::getInstance().createDynamicBody(0, 0);
        auto staticBody = World::getInstance().createStaticBody(10, 10);
        BOOST_CHECK_EQUAL(World::getInstance().getBodyCount(), 2);
        World::getInstance().destroyBody(dynamicBody);
        World::getInstance().destroyBody(staticBody);
        BOOST_CHECK_EQUAL(World::getInstance().getBodyCount(), 0);

    }

    BOOST_AUTO_TEST_CASE(check_if_chromosme_has_proper_size) {
        EvolutionaryAlgorithm algo(10, 2, 0.05);
        for (const auto& chromo : algo.GetCurrentGeneration()) {
            BOOST_CHECK_EQUAL(chromo.getBodyVertices().size(), Chromosome::VERTICES_NUMBER);
            BOOST_CHECK_EQUAL(chromo.getWheelVertices().size(), Chromosome::WHEELS_NUMBER);
            BOOST_CHECK_EQUAL(chromo.getWheelSizes().size(), Chromosome::WHEELS_NUMBER);
        }
    }

    BOOST_AUTO_TEST_CASE(check_if_evo_algo_generates_proper_generation) {
        size_t genSize = 10;
        EvolutionaryAlgorithm algo(genSize, 2, 0.05);

        BOOST_CHECK_EQUAL(algo.GetCurrentGeneration().size(), genSize);
    }


BOOST_AUTO_TEST_SUITE_END()