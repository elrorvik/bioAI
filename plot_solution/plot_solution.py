import string
import numpy as np
import matplotlib.pyplot as plt

def get_data_from_file(file_name):
    try:
        file = open(file_name,'r')
    except IOError:
        print("could not open file")
    data = []
    for line in file:
        numbers = line.split()
        data.append(numbers)
    data.append([])
    file.close()
    #for i in range(0,len(data)):
    #    print(data[i])
    return data



def get_depots(data):
    n_vehicles = int(data[0][0])  
    n_customers = int(data[0][1])
    n_depots = int(data[0][2])
    
    index = n_depots + n_customers + 1
    depot_xy = []
    
    for i in range(index,n_depots+index):
        depot_xy.append((data[i][1],data[i][2]))
    return depot_xy
    

def get_customers(data): 
    n_customers = int(data[0][1])
    n_depots = int(data[0][2])

    index = n_depots + 1
    customer_xy = []
    for i in range(index,n_customers+index):
        customer_xy.append((data[i][1],data[i][2]))
    return customer_xy

def get_vehicle_route(data):
    #print(data)
    route = []
    for i in range(1,len(data)-1):
        if(len(data[i])==0):
            continue;
        for j in range(4,len(data[i])):
            data[i][j] = int(data[i][j])
        #print(data[i])
        route.append((int(data[i][0]),data[i][4:]))
    return route


def draw_route(depot,customer,route,subplot_num):

    color = ['red','blue','orangered','turquoise','purple','red','salmon','sienna','silver','tan','pink','teal','tomato','violet','orchid','wheat','yellow','yellowgreen','aquamarine','azure','beige','plum','black','blue','brown','chartreuse','chocolate','coral','crimson','cyan','darkblue','darkgreen','fuchisia','gold','goldenrod','green','grey','indigo','ivory','khaki','lavender','lightblue','lightgreen','lime','magenta','marron','navy','olive','orange']
    for i in range(0,len(route)):
        d = depot[route[i][0]-1]
        temp = [];
        for j in range(0,len(route[i][1])):
            #print(route[i][1][j])
            if(route[i][1][j] == 0):
               temp.append([int(d[0]),int(d[1])])
            else:
               c = customer[route[i][1][j]-1]
               temp.append([int(c[0]),int(c[1])])
        
        data = np.array(temp)
        #print(data)
        #print(route[i][0])
        plt.subplot(subplot_num)
        plt.plot(data[:, 0], data[:, 1],linestyle='--', marker='o')
    
    
               
                         
def print_route(solution_name,info_name,subplot_num):
    info_data = get_data_from_file(info_name)
    solution_data = get_data_from_file(solution_name)
    #print(solution_data)
    depot_xy = get_depots(info_data)
    customer_xy = get_customers(info_data)
    route = get_vehicle_route(solution_data)
    draw_route(depot_xy,customer_xy,route,subplot_num)
            

solution_name="..//solution.txt"
info_name="..//testing_data//data_files//p03"
print_route(solution_name,info_name,211)
solution_name="..//testing_data//solution_files//p03.res"
print_route(solution_name,info_name,212)
plt.show()

