from z3 import *

# Define the inputs (32 switches)
switches = [Bool(f's{i}') for i in range(32)]

# Define intermediate variables for gate outputs
gate_outputs = {}
# Define the logic gates and their outputs
def and_gate(inputs, gate_name):
    output = Bool(f'{gate_name}_output')
    gate_outputs[gate_name] = output
    return And(inputs) == output

def or_gate(inputs, gate_name):
    output = Bool(f'{gate_name}_output')
    gate_outputs[gate_name] = output
    return Or(inputs) == output

def not_gate(input, gate_name):
    output = Bool(f'{gate_name}_output')
    gate_outputs[gate_name] = output
    return Not(input) == output

def xor_gate(input1, input2, gate_name):
    output = Bool(f'{gate_name}_output')
    gate_outputs[gate_name] = output
    return Xor(input1, input2) == output

# Define the connections between switches and gates
connections = {
    'not_gate_1' : [switches[16]],
    'not_gate_2' : [switches[18]],
    'not_gate_3' : [switches[2]],
    'not_gate_4' : [switches[20]],
    'not_gate_5' : [switches[4]],
    'not_gate_6' : [switches[22]],
    'not_gate_7' : [switches[24]],
    'not_gate_8' : [switches[8]],
    'and_gate_9' : [switches[5], switches[24]],
    'not_gate_10' : [switches[26]],
    'xor_gate_11' : [switches[5], switches[24]],
    'not_gate_12' : [switches[6]],
    'not_gate_13' : [switches[25]],
    'not_gate_14' : [switches[8]],
    'and_gate_15' : [switches[8], switches[27]],
    'not_gate_16' : [switches[7]],
    'not_gate_17' : [switches[26]],
    'not_gate_18' : [switches[12]],
    'and_gate_19' : [switches[9], switches[28]],
    'xor_gate_20' : [switches[8], switches[27]],
    'not_gate_21' : [switches[30]],
    'xor_gate_22' : [switches[9], switches[28]],
    'not_gate_23' : [switches[14]],
    'xor_gate_24' : [switches[11], switches[30]],
    'xor_gate_25' : [switches[13], switches[16]],
    'not_gate_26' : [switches[4]],
    'not_gate_27' : [switches[23]],
    'not_gate_28' : [switches[3]],
    'not_gate_29' : [switches[22]],
    'not_gate_30' : [switches[2]],
    'not_gate_31' : [switches[21]],
    'not_gate_32' : [switches[1]],
    'not_gate_33' : [switches[15]],
    'not_gate_34' : [switches[18]],
    'not_gate_35' : [switches[14]],
    'not_gate_36' : [switches[17]],
    'not_gate_37' : [switches[13]],
    'not_gate_38' : [switches[17]],
    'and_gate_39' : [switches[12], switches[15]],
    'not_gate_40' : [switches[12]],
    'and_gate_41' : [switches[11], switches[30]], 
    'not_gate_42' : [switches[10]],
    #'not_gate_43' : [gate_outputs['xor_gate_11']],
}

# Create a solver instance
solver = Solver()

# Add constraints based on connections
for gate, inputs in connections.items():
    if gate.startswith('and'):
        solver.add(and_gate(inputs, gate))
    elif gate.startswith('or'):
        solver.add(or_gate(inputs, gate))
    elif gate.startswith('not'):
        solver.add(not_gate(inputs[0], gate))
    elif gate.startswith('xor'):
        solver.add(xor_gate(inputs[0], inputs[1], gate))

# Check if the solver is satisfiable
if solver.check() == sat:
    model = solver.model()
    # Get the status of each switch
    switch_status = {str(switch): model[switch] for switch in switches}
    print("Switch Status:")
    for switch, status in switch_status.items():
        print(f"{switch}: {'On' if is_true(status) else 'Off'}")
else:
    print("No solution found.")