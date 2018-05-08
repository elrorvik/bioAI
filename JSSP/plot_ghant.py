# antall maskiner, antall job, makespan
# maskinid, jobid, start_time, end_time
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.patches as mpatches
import random


y_start = 0;
y_width = 6;

def make_gant_chart(n_machines, n_jobs, makespan,data,title):
    #colors_list = list(colors._colors_full_map.values())
    colors_list =  colors(n_jobs)
    fig, ax = plt.subplots()
    color_job = {}
    for i in range(n_machines):
        temp  = []
        color =[]
        for j in range(len(data[i])):
            [job_id,start_time,end_time] = data[i][j]
            temp.append((start_time,end_time-start_time))
            color.append(colors_list[job_id])
            if job_id not in color_job:
                color_job[job_id] = colors_list[job_id]
        ax.broken_barh(temp, (y_width*i, y_width), facecolors=color)  
    ax.set_ylim(0,n_machines*y_width)
    ax.set_xlim(0, makespan+0.2*makespan)
    ax.set_xlabel('time')
    ax.set_ylabel('machine num')
    label_place = [i for i in range(int(y_width/2),(n_machines)*y_width,y_width)]
    ax.set_yticks(label_place)
    label = [i for i in range(0,n_jobs)]
    handles =[]
    for i in range(0,n_jobs):
        red_patch = mpatches.Patch(color=color_job[i], label=i)
        handles.append(red_patch)
    plt.legend(handles=handles)
    plt.title(title)
    ax.set_yticklabels(label)
    ax.grid(True)
    title = ""; 
        
    #ax.set_title('axes title')



def get_solution(filename):
    try:
        textlist=open(filename).readlines()
    except:
        return
    data = {}
    for i in range(len(textlist)):
        tx = textlist[i]
        if(i == 0):
            n_machines,n_jobs,makespan=tx.split(',')
            n_jobs = int(n_jobs)
            makespan = int(makespan)
            n_machines = int(n_machines)
            continue
        if not tx.startswith('#'):
            machine_id,job_id,start_time,end_time = tx.split(',')
            machine_id = int(machine_id)
            job_id = int(job_id)
            start_time = int(start_time)
            end_time = int(end_time)
            if machine_id not in data:
                data[machine_id] = []
            data[machine_id].append([job_id,start_time,end_time])
    return n_machines, n_jobs, makespan,data

def colors(n):
    colors_list =  plt.get_cmap('tab20c')
    ret = []
    for i in range(n):
        ret.append(colors_list(i))
    return ret

filename = "plot\\ant_sol.txt"
n_machines, n_jobs, makespan,data = get_solution(filename)
make_gant_chart(n_machines, n_jobs, makespan,data,"ACO")

filename = "plot\\bee_sol.txt"
n_machines, n_jobs, makespan,data = get_solution(filename)
make_gant_chart(n_machines, n_jobs, makespan,data,"DABC")

plt.show()


                
            
