#!/usr/bin/env python3

import sys
import math
import numpy as np
import random
import operator
import pandas as pd

from common import print_solution, read_input


class City:
    def __init__(self, index, x, y):
        self.index = index
        self.x = x
        self.y = y

    def distance(self, city):
        xDis = abs(self.x - city.x)
        yDis = abs(self.y - city.y)
        distance = np.sqrt((xDis ** 2) + (yDis ** 2))
        return distance

    def __repr__(self):
        return "(" + str(self.x) + "," + str(self.y) + ")"

    def __str__(self):
        # when print
        return str(self.index)


class Fitness:
    def __init__(self, route):
        self.route = route
        self.distance = 0
        self.fitness = 0.0

    def routeDistance(self):
        if self.distance == 0:
            pathDistance = 0
            for i in range(0, len(self.route)):
                fromCity = self.route[i]
                toCity = None
                if i + 1 < len(self.route):
                    toCity = self.route[i + 1]
                else:
                    toCity = self.route[0]
                pathDistance += fromCity.distance(toCity)
            self.distance = pathDistance
        return self.distance

    def routeFitness(self):
        if self.fitness == 0:
            self.fitness = 1 / float(self.routeDistance())
        return self.fitness


def createRoute(cityList):
    route = random.sample(cityList, len(cityList))
    return route


def createRoute_greedy(cityList, current_city):
    N = len(cityList)

    dist = [[0] * N for i in range(N)]
    for i in range(N):
        for j in range(N):
            dist[i][j] = dist[j][i] = cityList[i].distance(cityList[j])

    route = [cityList[current_city]]
    unvisited_cities = set(range(0, N))
    unvisited_cities.remove(current_city)

    def distance_from_current_city(to):
        return dist[current_city][to]

    while unvisited_cities:
        next_city = min(unvisited_cities, key=distance_from_current_city)
        unvisited_cities.remove(next_city)
        route.append(cityList[next_city])
        current_city = next_city

    return route


def initialPopulation(popSize, cityList):
    population = []

    for i in range(0, popSize):
        population.append(createRoute_greedy(cityList, i % n))

    return population


def rankRoutes(population):
    fitnessResults = {}
    for i in range(0, len(population)):
        fitnessResults[i] = Fitness(population[i]).routeFitness()
    return sorted(fitnessResults.items(), key=operator.itemgetter(1), reverse=True)


def selection(popRanked, eliteSize):
    selectionResults = []
    df = pd.DataFrame(np.array(popRanked), columns=["Index", "Fitness"])
    df['cum_sum'] = df.Fitness.cumsum()
    df['cum_perc'] = 100*df.cum_sum/df.Fitness.sum()

    for i in range(0, eliteSize):
        selectionResults.append(popRanked[i][0])
    for i in range(0, len(popRanked) - eliteSize):
        pick = 100*random.random()
        for i in range(0, len(popRanked)):
            if pick <= df.iat[i, 3]:
                selectionResults.append(popRanked[i][0])
                break
    return selectionResults


def matingPool(population, selectionResults):
    matingpool = []
    for i in range(0, len(selectionResults)):
        index = selectionResults[i]
        matingpool.append(population[index])
    return matingpool


def breed(parent1, parent2):
    child = []
    childP1 = []
    childP2 = []

    geneA = int(random.random() * len(parent1))
    geneB = int(random.random() * len(parent1))

    startGene = min(geneA, geneB)
    endGene = max(geneA, geneB)

    for i in range(startGene, endGene):
        childP1.append(parent1[i])

    childP2 = [item for item in parent2 if item not in childP1]

    countAdd = 0
    iP1 = 0
    iP2 = 0
    while countAdd < len(parent1):
        if countAdd >= startGene and countAdd < endGene:
            child.append(childP1[iP1])
            iP1 += 1
        else:
            child.append(childP2[iP2])
            iP2 += 1
        countAdd += 1

    return child


def breedPopulation(matingpool, eliteSize):
    children = []
    length = len(matingpool) - eliteSize
    pool = random.sample(matingpool, len(matingpool))

    for i in range(0, eliteSize):
        children.append(matingpool[i])

    for i in range(0, length):
        child = breed(pool[i], pool[len(matingpool)-i-1])
        children.append(child)
    return children


def mutate(individual, mutationRate):
    for swapped in range(len(individual)):
        if(random.random() < mutationRate):
            swapWith = int(random.random() * len(individual))

            city1 = individual[swapped]
            city2 = individual[swapWith]

            individual[swapped] = city2
            individual[swapWith] = city1
    return individual


def mutatePopulation(population, mutationRateMin, mutationRateMax):
    mutatedPop = []
    popRanked = rankRoutes(population)
    F_avg = sum(x[1] for x in popRanked)/len(popRanked)
    F_max = popRanked[0][1]
    for ind in range(0, len(population)):
        F = Fitness(population[ind]).routeFitness()
        if F_max == F_avg:
            mutationRate = 0
        elif F < F_avg:
            mutationRate = mutationRateMax
        else:
            mutationRate = mutationRateMax * \
                (mutationRateMax-mutationRateMin)*(F-F_avg)/(F_max-F_avg)
        mutatedInd = mutate(population[ind], mutationRate)
        mutatedPop.append(mutatedInd)
    return mutatedPop


def nextGeneration(currentGen, eliteSize, mutationRateMin, mutationRateMax):
    popRanked = rankRoutes(currentGen)
    selectionResults = selection(popRanked, eliteSize)
    matingpool = matingPool(currentGen, selectionResults)
    children = breedPopulation(matingpool, eliteSize)
    nextGeneration = mutatePopulation(
        children, mutationRateMin, mutationRateMax)
    return nextGeneration


def geneticAlgorithm(population, popSize, eliteSize, mutationRateMin, mutationRateMax, generations):
    pop = initialPopulation(popSize, population)
    # print("Initial distance: " + str(1 / rankRoutes(pop)[0][1]))

    for i in range(0, generations):
        pop = nextGeneration(pop, eliteSize, mutationRateMin, mutationRateMax)

    # print("Final distance: " + str(1 / rankRoutes(pop)[0][1]))
    bestRouteIndex = rankRoutes(pop)[0][0]
    bestRoute = pop[bestRouteIndex]
    return bestRoute


if __name__ == '__main__':
    assert len(sys.argv) > 1
    filename = sys.argv[1]

    with open(filename) as f:
        cityList = []
        index = 0
        for line in f.readlines()[1:]:  # Ignore the first line.
            xy = line.split(',')
            cityList.append(City(index, float(xy[0]), float(xy[1])))
            index += 1
    n = index
    solution = geneticAlgorithm(
        population=cityList, popSize=100, eliteSize=10, mutationRateMin=0.01,
        mutationRateMax=0.05, generations=int(sys.argv[2]))
    print_solution(solution)
