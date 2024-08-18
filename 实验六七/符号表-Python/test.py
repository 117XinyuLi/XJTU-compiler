import re


class Parser:
    def __init__(self, code):
        self.code = code
        self.line_number = 0
        self.scope_stack = []
        self.scope_level = -1
        self.variables = []
        self.functions = [{'id': 0, 'name': 'None', 'return_type': 'void', 'scope': '/', 'params': [], 'head': -1, 'link': [-1]}]
        self.current_function = 0
        self.func_id = 0

    def parse(self):
        lines = self.code.split('\n')
        for line in lines:
            self.line_number += 1
            self.parse_line(line)

        # 将func id对应的function和variable关联起来
        func_vars = []
        for func in self.functions:
            func_var = {'function': func, 'variables': []}
            for var in self.variables:
                if var[-1] == func['id']:
                    func_var['variables'].append(var)
            func_vars.append(func_var)

        return func_vars

    def parse_line(self, line):
        # 去掉首尾空格
        line = line.strip()
        # 如果是空行，直接返回
        if not line:
            return
        # 如果是注释，直接返回
        if line.startswith('//'):
            return
        # 如果是作用域开始
        if line == '{':
            self.scope_level += 1
            self.scope_stack.append(self.line_number)
        # 如果是作用域结束
        elif line == '}':
            self.scope_level -= 1
            scope_start = self.scope_stack.pop()
            scope_end = self.line_number
            self.parse_scope(scope_start, scope_end)
            self.current_function = self.functions[self.current_function]['head']
        # 如果是return语句
        elif line.startswith('return'):
            return
        # 如果是函数定义
        elif '(' in line and ')' in line:
            self.parse_function(line)
        else:
            self.parse_variable(line)

    def parse_function(self, line):
        parts = line.split(' ')
        name, params = parts[1].split('(')
        return_type = parts[0]
        params += ' '
        for i in range(2, len(parts)):
            params += parts[i] + ' '
        params = params.split(')')[0]
        if len(params) == 0:
            params = []
        else:
            params = params.split(',')
        scope = self.scope_level if self.scope_level != -1 else 'global'
        # 连接上一层函数
        head = self.current_function
        # 新建函数
        self.func_id += 1
        self.functions.append({'id': self.func_id, 'name': name, 'return_type': return_type, 'scope': scope, 'params': params, 'head': head, 'link': [-1]})
        # 更新当前函数
        self.current_function = self.func_id
        # 更新上一层函数的link
        self.functions[head]['link'].append(self.func_id)
        if self.functions[head]['link'][0] == -1:
            self.functions[head]['link'].pop(0)

    def parse_scope(self, scope_start, scope_end):
        for variable in self.variables:
            if variable[2] == scope_start:
                variable[2] = self.scope_level + 1

    def parse_variable(self, line):
        # 用空格分割
        parts = line.split(' ')
        # 如果长度小于2，说明不是变量定义
        if len(parts) < 2:
            raise ValueError(f"Invalid variable definition at line {self.line_number}.")
        # 如果有const关键字
        if parts[0] == 'const':
            constant = True
            parts = parts[1:]
        else:
            constant = False
        # 如果有[]，说明是数组
        if '[' in parts[1]:
            array = True
        else:
            array = False
        # 获取变量名
        var_name = re.match(r'^[a-zA-Z_]\w*', parts[1])
        if not var_name:
            raise ValueError(f"Invalid variable name at line {self.line_number}.")
        var_name = var_name.group(0)
        # 获取变量类型
        var_type = parts[0]
        # 获取作用域范围
        if len(self.scope_stack) != 0:
            scope = self.scope_stack[-1]
        else:
            scope = 'global'
        # 如果是数组，获取数组大小
        if array:
            size = parts[1].split('[')[1].split(']')[0]
            size = size.split(',')
            size = [int(s) for s in size]
        else:
            size = None
        # 添加变量
        if array:
            self.variables.append([var_name, var_type, scope, constant, array, size, self.current_function])
        else:
            self.variables.append([var_name, var_type, scope, constant, array, self.current_function])

class SymbolTable:
    def __init__(self):
        self.function = {}
        self.table = {}

    def set_function(self, function):
        self.function = function
        # 将函数的参数变为字符串
        self.function['params'] = ', '.join(self.function['params']) if len(self.function['params']) > 0 else 'None'
        # 将函数link变为字符串
        self.function['link'] = ', '.join([str(l) for l in self.function['link']])

    def add_variable(self, name, var_type, scope, constant, array):
        if name in self.table:
            raise ValueError(f"Variable {name} already exists.")
        self.table[name] = {'type': var_type, 'scope': scope, 'constant': constant, 'array': array}

    def add_array(self, name, var_type, scope, constant, array, size):
        if name in self.table:
            raise ValueError(f"Variable {name} already exists.")
        self.table[name] = {'type': var_type, 'scope': scope, 'constant': constant, 'array': array, 'size': size}
        # 将数组大小变为字符串
        self.table[name]['size'] = ', '.join([str(s) for s in self.table[name]['size']])

    def get_variable_info(self, name):
        if name not in self.table:
            raise ValueError(f"Variable {name} does not exist.")
        return self.table[name]

    def check_type(self, name, expected_type):
        var_info = self.get_variable_info(name)
        if var_info['type'] != expected_type:
            raise TypeError(f"Variable {name} is of type {var_info['type']}, but expected {expected_type}.")


# Read code from file
with open('code.txt', 'r') as file:
    code = file.read()

# Parse the code
parser = Parser(code)
func_vars = parser.parse()

# Add variables to symbol table
symbol_tables = []
for func_var in func_vars:
    symbol_table = SymbolTable()
    symbol_table.set_function(func_var['function'])
    for var in func_var['variables']:
        if var[4]:
            symbol_table.add_array(var[0], var[1], var[2], var[3], var[4], var[5])
        else:
            symbol_table.add_variable(var[0], var[1], var[2], var[3], var[4])

    symbol_tables.append(symbol_table)

# 检查重复的函数名
function_names = [symbol_table.function['name'] for symbol_table in symbol_tables]
if len(function_names) != len(set(function_names)):
    raise ValueError("Duplicate function names.")

# Get variable info
for symbol_table in symbol_tables:
    print('Function:', symbol_table.function['name'])
    # 输出函数信息：ID, Return Type, Scope, Params, Head, Link，分别占用10，20，15，20，10，10个字符，左对齐
    print(f"{'ID':<10}{'Return Type':<20}{'Scope':<15}{'Params':<20}{'Head':<10}{'Link':<10}")
    print(f"{symbol_table.function['id']:<10}{symbol_table.function['return_type']:<20}{symbol_table.function['scope']:<15}{symbol_table.function['params']:<20}{symbol_table.function['head']:<10}{symbol_table.function['link']:<10}")

    print('\nVariables:')
    # 输出变量信息：Name, Type, Scope, Constant, Array, Size，分别占用10，15，15，15，15，10个字符，左对齐
    print(f"{'Name':<10}{'Type':<15}{'Scope':<15}{'Constant':<15}{'Array':<15}{'Size':<10}")
    for name, info in symbol_table.table.items():
        if info['array']:
            print(f"{name:<10}{info['type']:<15}{info['scope']:<15}{info['constant']:<15}{info['array']:<15}{info['size']:<10}")
        else:
            print(f"{name:<10}{info['type']:<15}{info['scope']:<15}{info['constant']:<15}{info['array']:<15}{'/':<10}")

    # 输出分割线
    print('-' * 80)
