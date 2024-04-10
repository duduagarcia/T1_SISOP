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
def cozinheiro():
    global count
    while True:
        cozinha.acquire() # wait cozinha
        enche_travessa()
        for _ in range(M):
            comida.release() # signal comida
        count = M
        enchendo.release()

# canibal
def canibal():
    global count
    while True:
        comida.acquire() # wait comida
        mutex.acquire() # wait mutex
        count -= 1 # diminui uma porção 
        if count == 0: # se nao tiver mais porções
            cozinha.release() # signal cozinha 
            enchendo.acquire() # wait enchendo
        mutex.release() 
        come()

# Função para simular o ato de comer
def come():
    print("Canibal come")

# Função para simular a ação de encher a travessa
def enche_travessa():
    print("Cozinheiro enche o pote")

# Função para iniciar o jantar dos canibais
def jantar_canibais():
    cozinheiro_thread = Thread(target=cozinheiro)

    canibal_threads = []
    for _ in range(N):
        canibal_thread = Thread(target=canibal)
        canibal_threads.append(canibal_thread)
        canibal_thread.start()

    cozinheiro_thread.start()

jantar_canibais()