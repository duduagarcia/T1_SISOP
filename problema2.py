from threading import Semaphore, Thread

# parâmetros de entrada
NUM_SAVAGE = 5
NUM_SERVING = 10

# Semáforos e variáveis
mutex = Semaphore(1)
full_pot = Semaphore(0)
empty_pot = Semaphore(0)
servings = 0

# cozinheiro
def cook():
    while True:
        empty_pot.acquire()
        full_pot.release()
        print("Cozinheiro enche o pote")

# canibal
def savage(servings_size: int, servings):
    while True:
        mutex.acquire()
        if servings == 0:
            empty_pot.release()
            full_pot.acquire()
            servings = servings_size
        servings -= 1
        print("Canibal come")
        mutex.release()

# Função para iniciar o jantar dos canibais
def dining_savages():
    cook_thread = Thread(target=cook)

    for i in range(NUM_SAVAGE):
        savage_thread = Thread(target=savage, args=(NUM_SERVING, servings))
        savage_thread.start()

    cook_thread.start()

# Iniciar o jantar dos canibais
dining_savages()
