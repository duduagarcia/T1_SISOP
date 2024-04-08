from threading import Semaphore, Thread

# parâmetros de entrada
N = 5 # numero de canibais
M = 10  # número de porções que o cozinheiro adiciona à travessa

# Semáforos e variáveis
cozinha = Semaphore(0)  
enchendo = Semaphore(0)
comida = Semaphore(1)
mutex = Semaphore(1)
count = 1

# cozinheiro
def cook():
    global count
    while True:
        cozinha.acquire() # wait cozinha
        enche_travessa()
        for _ in range(M):
            comida.release()
        count = M
        enchendo.release()

# canibal
def savage():
    global count
    while True:
        comida.acquire()
        mutex.acquire()
        count -= 1
        if count == 0:
            cozinha.release() # signal
            enchendo.acquire()
        mutex.release()
        come()

# Função para simular o ato de comer
def come():
    print("Canibal come")

# Função fictícia para encher a travessa
def enche_travessa():
    print("Cozinheiro enche o pote")

# Função para iniciar o jantar dos canibais
def dining_savages():
    cook_thread = Thread(target=cook)

    savage_threads = []
    for _ in range(N):
        savage_thread = Thread(target=savage)
        savage_threads.append(savage_thread)
        savage_thread.start()

    cook_thread.start()

# Iniciar o jantar dos canibais
dining_savages()
