

class father:
    def __init__(self, count):
        self.count = count

    def increment(self):
        self.count += 1
        print('Father count {}'.format(self.count))


class child(father):
    def __init__(self):
        super().__init__(count=0)

    def increment(self):
        print('Child pass')
        super().increment()


child = child()
child.increment()


