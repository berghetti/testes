from socket import *
import ipaddress
import argparse

'''timeout da conexao do socket'''
timeout = 0.2


def args():
    parse = argparse.ArgumentParser(prog='scan')
    parse.add_argument('destino', help='especificar destino, ex: localhost')
    parse.add_argument('-p', default='21,22,23,53,80,443,3389', help='definir portas, ex: -p 80,443,8080')
    args = parse.parse_args()
    destino = args.destino
    portas = args.p.split(',')
    return destino, portas


def alvo(destino):
    try:
        rede = gethostbyname(destino)
    except Exception:
        rede = destino

    try:
        network = ipaddress.IPv4Interface(rede).network
        hosts = list(ipaddress.IPv4Network(network).hosts())
        return hosts, rede
    except Exception:
        print('\033[31m \nendereço nao encontrado\n')
        exit(1)


def scan(hosts, rede, portas):
    hosts = hosts
    host = rede
    # caso só tenha um host para scanear
    if len(hosts) == 0:
        ip = ipaddress.IPv4Interface(host).ip
        print('Scaning... {}\n'.format(ip))
        for porta in portas:
            s = socket(AF_INET, SOCK_STREAM)
            s.settimeout(timeout)
            cod = s.connect_ex((str(ip), int(porta)))
            if cod == 0:
                print('\033[32m[!]\033[0;0m {}:{} ===> OPEN'.format(ip, porta))
            s.close()
        print('\nconcluido')
    else:
        print('Scaning...{}\n'.format(rede))
        for ip in range(0, len(hosts)):
            for porta in portas:
                s = socket(AF_INET, SOCK_STREAM)
                s.settimeout(timeout)
                cod = s.connect_ex((str(hosts[ip]), int(porta)))
                if cod == 0:
                    print('\033[32m[!]\033[0;0m {}:{} ===> OPEN'.format(hosts[ip], porta))
                s.close()
        print('\nconcluido')


def main():
    destino, portas = args()
    hosts, rede = alvo(destino)
    scan(hosts, rede, portas)


if __name__ == '__main__':
    main()
