from pulp import *

def solve_and_print_results():
    # Read the first line to get information about the size of the lists and max production
    num_toys, num_packages, max_toys_per_day = map(int, input().split())

    #define problem
    prob = LpProblem("Christmas Toy Factory", LpMaximize)

    #vectors that hold the number of packages with the respective toy
    toy_vector = [0] * (num_toys + 1)

    # Read information about individual toys
    toy_vars = {}
    toy_profits = {}
    toy_capacities = {}
    for i in range(1, num_toys + 1):
        profit, capacity = map(int, input().split())
        toy_profits[i] = profit
        toy_capacities[i] = capacity
        toy_vars[i] = LpVariable(f'Toy_{i}', 0, toy_capacities[i], cat='Integer')


    # Read information about special packages
    package_vars = {}
    package_profits = {}
    package_contents = {}
    for p in range(1, num_packages + 1):
        toy1, toy2, toy3, profit = map(int, input().split())
        package_vars[p] = LpVariable(f'Package_{p}', 0, min(toy_capacities[toy1], toy_capacities[toy2], toy_capacities[toy3]),cat='Integer')
        package_profits[p] = profit
        package_contents[p] = (toy1, toy2, toy3)
        #Introduce the package_var in their respective vector
        toy_vector[toy1]+=package_vars[p]
        toy_vector[toy2]+=package_vars[p]
        toy_vector[toy3]+=package_vars[p]

    #Objective:
    prob+= lpSum(package_profits[i] * package_vars[i] for i in range(1, num_packages + 1)) + lpSum(toy_profits[e] * toy_vars[e] for e in range(1, num_toys + 1))

    #Condition for number of toys in package
    for i in range(1, num_toys + 1):
        prob += toy_vars[i] + lpSum(toy_vector[i]) <= toy_capacities[i]

    # Conditions for overall selling of toys
    prob += (lpSum(toy_vars[i] for i in range(1, num_toys + 1)) + lpSum(package_vars[j]*3 for j in range(1, num_packages + 1))) <= max_toys_per_day

    # Solve the problem
    prob.solve(GLPK(msg=False))
    print(int(value(prob.objective)))

if __name__ == "__main__":
    # Call the function to solve the problem and print results
    solve_and_print_results()
