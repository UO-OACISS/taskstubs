#!/usr/bin/env python3

import networkx as nx
import gzip
import json
import sys
import argparse
import os
import glob
import matplotlib.pyplot as plt


parser = argparse.ArgumentParser(description="Analyze APEX Google Trace Event trace files.")
parser.add_argument("--compressed", dest="compressed", action="store_true", default=True,
                    help="Read trace_events.*.json.gz files (default: read trace_events.*.json files)")
parser.add_argument("--ascii", dest="ascii", action="store_true", default=False,
                    help="Print graph as ASCII tree")
parser.add_argument("--draw", dest="draw", action="store_true", default=False,
                    help="Draw critical-path graph as networkx graph")
parser.add_argument("--verbose", dest="verbose", action="store_true", default=False,
                    help="Print critical-path nodes")
parser.add_argument("--main-task", dest="main_task", default="0", required=False, type=int,
                    help="The main task GUID")
parser.add_argument("--filename", dest="filename", default=None, required=False, type=str, help="The filename to parse (default is trace_events.*.json.gz)")

def critical_path_method(graph, root):
    """
    Calculates the critical path of a project represented as a directed acyclic graph.

    Args:
        graph: A networkx DiGraph representing the project, with task durations as node attributes ("duration").

    Returns:
        A tuple containing:
            - The critical path as a list of task names.
            - The project completion time.
    """
    starttime = graph.nodes[root]["ts"]
    endtime = starttime + graph.nodes[root]["duration"]

    # Forward pass
    for node in nx.topological_sort(graph):
        est = 0
        for predecessor in graph.predecessors(node):
            est = max(est, graph.nodes[predecessor]["eft"])
        graph.nodes[node]["est"] = est
        graph.nodes[node]["eft"] = est + graph.nodes[node]["duration"]
        tmpend = graph.nodes[node]["ts"] + graph.nodes[node]["duration"]
        if tmpend > endtime:
            #print(graph.nodes[node]["guid"],"ends at",tmpend)
            endtime = tmpend

    # Backward pass
    for node in reversed(list(nx.topological_sort(graph))):
        if not list(graph.successors(node)):
            graph.nodes[node]["lft"] = graph.nodes[node]["eft"]
        else:
            lft = float("inf")
            for successor in graph.successors(node):
                lft = min(lft, graph.nodes[successor]["lst"])
            graph.nodes[node]["lft"] = lft
        graph.nodes[node]["lst"] = graph.nodes[node]["lft"] - graph.nodes[node]["duration"]

    # Calculate float and identify critical path
    critical_path = []
    project_completion_time = 0
    for node in graph.nodes:
      graph.nodes[node]["float"] = graph.nodes[node]["lst"] - graph.nodes[node]["est"]
      if graph.nodes[node]["float"] == 0:
          critical_path.append(node)
      project_completion_time = max(project_completion_time, graph.nodes[node]["eft"])

    actual = endtime - starttime
    return critical_path, actual, project_completion_time

# This function creates a test graph
def dummy():
    # Create an empty graph
    G = nx.DiGraph()

    tasks = {
        "A": 4,
        "B": 3,
        "C": 5,
        "D": 2,
        "E": 6,
        "F": 3
    }

    dependencies = {
        "B": ["A"],
        "C": ["A"],
        "D": ["B", "C"],
        "E": ["D"],
        "F": ["C"]
    }

    # Add nodes
    for key,value in tasks.items():
        G.add_node(key, duration=value)

    for key,value in dependencies.items():
        for child in value:
            G.add_edge(key,child)

    # Print nodes and edges
    print("Nodes:", G.nodes())
    print("Edges:", G.edges())
    return G

def readGraph(inargs):

    if inargs.filename:
        myglob = inargs.filename
    else:
        if inargs.compressed:
            myglob = "trace_events.[0-9]*.json.gz"
        else:
            myglob = "trace_events.[0-9]*.json"

    tracefiles = glob.glob(myglob);
    if len(tracefiles) == 0:
        print("No files found!")
        if inargs.compressed:
            print("  Are you sure they're compressed?")
        else:
            print("  Are they compressed?  If so, please use the --compressed argument")
        parser.print_usage()
        sys.exit(1)

    # Create an empty graph
    G = nx.DiGraph()
    begin = 0
    end = 0
    firstCreate = 0
    lastDestroy = 0

    for counter, infile in enumerate(sorted(glob.glob(myglob))):
        print("\nReading",infile)
        #with open (infile, 'r') as jsonfile:
        if inargs.compressed:
            jsonfile = gzip.open(infile, "r")
        else:
            jsonfile = open(infile, "r")
        data = json.load(jsonfile)

        for line in data["traceEvents"]:
            """
            # look for this:
            # {"name":"APEX Trace Begin","ph":"R","pid":0,"tid":0,"ts":1739400926808546.250},
            if line["ph"] == "R" and line["name"] == "APEX Trace Begin":
                if begin == 0:
                    begin = line["ts"]
                else:
                    begin = min(begin,line["ts"])
                continue
                
            # look for this:
            # {"name":"APEX Trace End", "ph":"R","pid":0,"tid":0,"ts":1739400930884812.250}
            if line["ph"] == "R" and line["name"] == "APEX Trace End":
                end = max(end,line["ts"])
                continue
            """

            # look for this:
            # {"name":"Task Creation","ph":"R","pid":0,"tid":0,"ts":1741629788031981.250,"args":{"GUID":144678138029277185,"name":"APPLY_DIAG"}},
            if firstCreate == 0 and line["ph"] == "R" and line["name"] == "Task Creation":
                firstCreate = line["ts"]
                continue

            # look for this:
            # {"name":"Task Destroy","ph":"R","pid":0,"tid":0,"ts":1741629788204219.750,"args":{"GUID":2017612633061982208,"name":"potrf_dpotrf"}},
            if line["ph"] == "R" and line["name"] == "Task Destroy":
                lastDestroy = line["ts"]
                continue
                
            # look for this:
            # {"name":"cudaGetDeviceCount","cat":"CPU","ph":"X","pid":0,"tid":0,"ts":1739400928607165.500,"dur":120.250,"args":{"Implicit Parent":false,"GUID":2,"Parent GUID":1}},
            if line["ph"] == "X":
                args = line["args"]
                name = line["name"]
                if "Implicit Parent" in args:
                    if args["Implicit Parent"] and "apex_preload_main" not in name:
                        continue
                dur = line["dur"]
                ts = line["ts"]
                if "apex_preload_main" in name:
                    duration = int(dur)
                    begin = int(ts)
                    dur = 0
                guid = args["GUID"]
                pguid = args["Parent GUID"]
                # Set the "main" timer length to 0, so that it doesn't distort our analysis
                #if (guid == inargs.main_task):
                    #dur = 0.0
                G.add_node(guid, guid=guid, name=name, duration=dur, args=args, pguid=pguid, ts=ts)
                if isinstance(pguid, list):
                    for P in pguid:
                        if P >= inargs.main_task:
                            G.add_edge(P,guid)
                else:
                    if pguid >= inargs.main_task:
                        G.add_edge(pguid,guid)

        jsonfile.close()
    if 0 in G.nodes and G.nodes[0] == {}:
        G.add_node(0, guid=0, name="APEX MAIN", duration=0, args={"GUID":0,"Parent GUID":-1}, pguid=-1, ts=firstCreate)

    #duration = (end - begin) * 1.0e-6
    duration = duration * 1.0e-6
    path = (lastDestroy - firstCreate) * 1.0e-6
    print("Graph has",G.number_of_nodes(),"nodes and",G.size(),"edges.")
    print("Program took",duration,"seconds.")
    print("Total tasks lifetime",path,"seconds.")
    return G

def get_subgraph_from_node(graph, start_node):
    if start_node not in graph:
        raise ValueError("Start node is not in the graph.")

    nodes = set()
    queue = [(start_node, 0)]  # (node, level)
    while queue:
        current_node, current_level = queue.pop(0)
        nodes.add(current_node)

        for neighbor in graph.neighbors(current_node):
            if neighbor not in nodes:
                queue.append((neighbor, current_level + 1))
    return graph.subgraph(nodes).copy()

def main():
    inargs = parser.parse_args()
    # load the trace as a graph
    # G = dummy()
    fullG = readGraph(inargs)
    G = get_subgraph_from_node(fullG, inargs.main_task)

    critical_path, actual, project_completion_time = critical_path_method(G, inargs.main_task)

    last_node = str(critical_path[-1])
    if inargs.ascii:
        ascii_graph = nx.generate_network_text(G)
        for line in ascii_graph:
            print(line)
            if last_node in line:
                print("...")
                break

    print("Critical path starting at node:", inargs.main_task)
    print("    has",G.number_of_nodes(),"nodes and",G.size(),"edges.")
    if inargs.verbose:
        print("Critical path nodes:",critical_path)
    print("Critical path length:",len(critical_path))
    print("Critical path measured time:",actual * 1.0e-6, "seconds")
    print("Critical path optimal time:",project_completion_time * 1.0e-6,"seconds")

    if inargs.draw:
        #nx.draw(G,with_labels=True,nodelist=critical_path,font_size=6)
        nx.draw(G,with_labels=True,font_size=6,pos=nx.spring_layout(G))
        plt.draw()
        plt.show()


if __name__ == "__main__":
    main()
