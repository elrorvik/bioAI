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
    file.close()
    #for i in range(0,len(data)):
    #    print(data[i])
    return data

def get_depots(data):
    n_vehicles = data[0][0]
    n_vehicles = int(n_vehicles)
    
    n_customers = int(data[0][1])
    n_depot = data[0][2]
    n_depot = int(n_depot)

    #print("tull", data[0][5])
    print("n_vehicles", n_vehicles)
    print("n_customers",n_customers)
    print("n_depots",n_depot)
    
    index = n_vehicles + n_customers + 1

    depot_xy = []
    for i in range(index,n_depot+index):
        depot_xy.append((data[i][1],data[i][2]))
    return depot_xy

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
    route = []
    for i in range(1,len(data)-1):
        for j in range(4,len(data[i])):
            data[i][j] = int(data[i][j])
        route.append((int(data[i][0]),data[i][4:]))
    return route

def draw_route(depot,customer,route):
    color = ['orangered','orchid','pink','purple','red','salmon','sienna','silver','tan','teal','tomato','turquoise','violet','wheat','yellow','yellowgreen','aquamarine','azure','beige','plum','black','blue','brown','chartreuse','chocolate','coral','crimson','cyan','darkblue','darkgreen','fuchisia','gold','goldenrod','green','grey','indigo','ivory','khaki','lavender','lightblue','lightgreen','lime','magenta','marron','navy','olive','orange']
    for i in range(0,len(route)):
        
        d = depot[route[i][0]-1]
        plt.scatter(d[0],d[1],color = color[route[i][0]-1])
        for j in range(0,len(route[i][1])-1):
            if(j == 0):
               c1 = customer[route[i][1][j+1]-1]
               plt.plot([c1[0],d[0]],[c1[1],d[1]],color = color[route[i][0]-1])
               #print( route[i][0]-1 , " depot to ",  route[i][1][j+1]," ",d, " to ",c1 )
            elif(j == len(route[i][1])-2):
               c1 = customer[route[i][1][j]-1]
               plt.plot([c1[0],d[0]],[c1[1],d[1]],color = color[route[i][0]-1])
               #print( route[i][1][j] , " to ", route[i][0]-1, " ",c1 , " to ", d)
            else:
               c1 = customer[route[i][1][j]-1]
               c2 = customer[route[i][1][j+1]-1]
               plt.plot([c1[0],c2[0]],[c1[1],c2[1]],color = color[route[i][0]-1])
               #print( route[i][1][j] , " to ", route[i][1][j+1]," ",c1, " to ",c2)
    plt.show()
               
                         
                        
            
solution_name="..//testing_data//solution_files//p02.res"
info_name="..//testing_data//data_files//p02"
info_data = get_data_from_file(info_name)
solution_data = get_data_from_file(solution_name)
print(solution_data)
depot_xy = get_depots(info_data)
customer_xy = get_customers(info_data)
route = get_vehicle_route(solution_data)
draw_route(depot_xy,customer_xy,route)
