class Quadruple:
    def __init__(self, op, arg1, arg2, result):
        self.op = op
        self.arg1 = arg1
        self.arg2 = arg2
        self.result = result


class SemanticAnalyzer:
    def __init__(self):
        self.quadruples = []
        self.stack = []

    def gen(self, op, arg1, arg2, result):
        quad = Quadruple(op, arg1, arg2, result)
        self.quadruples.append(quad)
        return len(self.quadruples) - 1

    def backpatch(self, list, quad):
        for i in list:
            self.quadruples[i].result = quad
    def analyze(self, code):
        lines = code.split('\n')
        for line in lines:
            if 'while' in line and 'do' in line:
                condition = line.split('while')[1].split('do')[0].strip()
                self.stack.append(self.gen('jfalse', condition, '_', '_'))
            elif 'if' in line and 'then' in line:
                condition = line.split('if')[1].split('then')[0].strip()
                self.stack.append(self.gen('jfalse', condition, '_', '_'))
            elif 'end' in line and ('if' in line or 'while' in line):
                quad = self.stack.pop()
                if 'if' in line:
                    self.backpatch([quad], len(self.quadruples))
                elif 'while' in line:
                    self.backpatch([quad], len(self.quadruples) + 1)
                # 根据情况判断是否为if语句还是while语句
                if 'if' in line:
                    self.stack.append(self.gen('goto', '_', '_', '_'))
                elif 'while' in line:
                    self.gen('goto', '_', '_', quad)
            elif 'else' in line:
                quad = self.stack.pop()
                self.stack.append(self.gen('goto', '_', '_', '_'))
                self.backpatch([quad], len(self.quadruples))
            else:
                # 赋值语句
                parts = line.split(':=')
                result = parts[0].strip()
                expr = parts[1].strip()
                self.gen('=', expr, '_', result)


with open('code.txt', 'r') as f:
    code = f.read()

analyzer = SemanticAnalyzer()
analyzer.analyze(code)

start = 0
for quad in analyzer.quadruples:
    print(f'{start} ({quad.op}, {quad.arg1}, {quad.arg2}, {quad.result})')
    start += 1

