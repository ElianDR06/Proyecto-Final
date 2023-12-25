/*----------------------------------------------------------------
|  Autor: Elian A. Dominguez            Matricula: 1-18-0536      |
|  Fecha: 15/12/2023                              Version: 1.83.1 |
|-----------------------------------------------------------------|
|  Descripcion del Programa: Este es un programa en C             |
|  simulando una maquina de snack donde se tienen 10 productos    |
|  a seleccionar, dando como resultado el precio, una descripcion |  
|  el peso y las calorias                                         |
| ----------------------------------------------------------------*/


// Incluir E/S y Librerias Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definicion de las estructuras
struct Producto {
    char nombre[50];
    char descripcion[100];
    float precio;
    float peso;
    int calorias;
    int cantidad;
};

struct Usuario {
    char nombre[50];
    char contrasena[50];
};

bool buscarUsuario(struct Usuario usuarios[], int numUsuarios, const char* nombre) {
    for (int i = 0; i < numUsuarios; i++) {
        if (strcmp(usuarios[i].nombre, nombre) == 0) {
            return true; // El usuario existe
        }
    }
    return false; // El usuario no existe
}

void cargarProductos(struct Producto productos[], int* numProductos);
void agregarUsuario(struct Usuario usuarios[], int* numUsuarios, const char* nombre, const char* contrasena) {
    strcpy(usuarios[*numUsuarios].nombre, nombre);
    strcpy(usuarios[*numUsuarios].contrasena, contrasena);
    (*numUsuarios)++;
}
void guardarUsuarios(struct Usuario usuarios[], int numUsuarios) {
    FILE* archivo = fopen("usuarios.txt", "w");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de usuarios.\n\n");
        return;
    }
    for (int i = 0; i < numUsuarios; i++) {
        fprintf(archivo, "Nombre: %s\n", usuarios[i].nombre);
        fprintf(archivo, "Contrasena: %s\n", usuarios[i].contrasena);
        fprintf(archivo, "\n");
    }
     fclose(archivo);
}

void cargarUsuarios(struct Usuario usuarios[], int* numUsuarios) {
    FILE* archivo = fopen("usuarios.txt", "r");

    if (archivo == NULL) {
        return;
    }

    char linea[200];

    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "Nombre: %[^\n]", usuarios[*numUsuarios].nombre);
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "Contrasena: %[^\n]", usuarios[*numUsuarios].contrasena);
        fgets(linea, sizeof(linea), archivo);
        fgets(linea, sizeof(linea), archivo); 
        (*numUsuarios)++;
    }

    fclose(archivo);
}

void mostrarMenu();
void iniciarSesion(struct Usuario usuarios[], int* numUsuarios, struct Usuario* usuario);
void cambiarContrasena(struct Usuario* usuario);
void agregarProducto(struct Producto productos[], int* numProductos);
void mostrarContabilidad(struct Producto productos[], int numProductos);
void mostrarProductos(struct Producto productos[], int numProductos);
void realizarVenta(struct Producto productos[], int numProductos);
void guardarProductos(struct Producto productos[], int numProductos);
void cargarProductos(struct Producto productos[], int* numProductos);  


int main() {
    int opcion;
    int numUsuarios = 0;
    struct Usuario usuarios[10]; 
    struct Usuario usuario;
    int numProductos = 0;
    struct Producto productos[17];

    cargarUsuarios(usuarios, &numUsuarios);
    cargarProductos(productos, &numProductos);

    while (1) {
        mostrarMenu();
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        getchar(); 

        switch (opcion) {
            case 1:
                iniciarSesion(usuarios, &numUsuarios, &usuario);
                break;
            case 2:
                cambiarContrasena(&usuario);
                break;
            case 3:
                if (strlen(usuario.nombre) == 0) {
                    printf("Debe iniciar sesion primero.\n\n");
                } else {
                    agregarProducto(productos, &numProductos);
                }
                break;
            case 4:
                if (strlen(usuario.nombre) == 0) {
                    printf("Debe iniciar sesion primero.\n\n");
                } else {
                    mostrarContabilidad(productos, numProductos);
                }
                break;
            case 5:
                mostrarProductos(productos, numProductos);
                break;
            case 6:
                realizarVenta(productos, numProductos);
                break;
            case 7:
                guardarProductos(productos, numProductos);
                printf("Se han guardado exitosamente los datos.\n\n");
                break;
            case 8:
                guardarUsuarios(usuarios, numUsuarios);
                printf("Se han guardado exitosamente los usuarios.\n");
                printf("Hasta luego.\n");
                return 0;
            default:
                printf("Opcion invalida. Intente de nuevo.\n\n");
        }
    }

    return 0;
}

void mostrarMenu() {
    printf("---- Menu ----\n");
    printf("1. Iniciar sesion\n");
    printf("2. Cambiar contrasena\n");
    printf("3. Agregar producto\n");
    printf("4. Mostrar contabilidad\n");
    printf("5. Mostrar productos\n");
    printf("6. Realizar venta\n");
    printf("7. Guardar productos\n");
    printf("8. Salir\n\n");
}

void iniciarSesion(struct Usuario usuarios[], int* numUsuarios, struct Usuario* usuario) {
    char nombre[50];
    char contrasena[50];

    printf("Ingrese su nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0'; 
    printf("Ingrese su contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0'; 

    // Verificar si el usuario existe
    if (buscarUsuario(usuarios, *numUsuarios, nombre)) {
        // Si el usuario existe, verificar la contraseña
        for (int i = 0; i < *numUsuarios; i++) {
            if (strcmp(usuarios[i].nombre, nombre) == 0 && strcmp(usuarios[i].contrasena, contrasena) == 0) {
               // Iniciar sesión
                strcpy(usuario->nombre, nombre);
                strcpy(usuario->contrasena, contrasena);
                printf("Sesion iniciada correctamente.\n\n");
                return;
            }
        }
        
        printf("Contrasena incorrecta.\n\n");
    } else {
        // Si el usuario no existe, ofrecer crear uno nuevo
        char opcion;
        printf("El usuario no existe. ¿Desea crear un nuevo usuario? (S/N): ");
        scanf(" %c", &opcion);
        if (opcion == 'S' || opcion == 's') {
            // Agregar el nuevo usuario
            agregarUsuario(usuarios, numUsuarios, nombre, contrasena);
            strcpy(usuario->nombre, nombre);
            strcpy(usuario->contrasena, contrasena);
            printf("Nuevo usuario creado y sesion iniciada correctamente.\n\n");
        } else {
            printf("Operacion cancelada.\n\n");
        }
    }
}



void cambiarContrasena(struct Usuario* usuario) {
    char contrasenaAntigua[50];
    
    if (strlen(usuario->nombre) == 0) {
        printf("Debe iniciar sesion primero.\n\n");
        return;
    }
    
    printf("Ingrese su contrasena actual: ");
    fgets(contrasenaAntigua, sizeof(contrasenaAntigua), stdin);
    contrasenaAntigua[strcspn(contrasenaAntigua, "\n")] = '\0'; 
    
    if (strcmp(usuario->contrasena, contrasenaAntigua) == 0) {
        char nuevaContrasena[50];
        
        printf("Ingrese su nueva contrasena: ");
        fgets(nuevaContrasena, sizeof(nuevaContrasena), stdin);
        nuevaContrasena[strcspn(nuevaContrasena, "\n")] = '\0'; 
        
        strcpy(usuario->contrasena, nuevaContrasena);
        printf("Contrasena cambiada exitosamente.\n\n");
    } else {
        printf("Contrasena incorrecta.\n\n");
    }
}

void agregarProducto(struct Producto productos[], int* numProductos) {
    if (*numProductos == 17) {
        printf("Ya se ha alcanzado el maximo de productos.\n\n");
        return;
    }
    
    struct Producto producto;
    
    printf("Ingrese el nombre del producto: ");
    fgets(producto.nombre, sizeof(producto.nombre), stdin);
    producto.nombre[strcspn(producto.nombre, "\n")] = '\0'; 
    
    printf("Ingrese la descripcion del producto: ");
    fgets(producto.descripcion, sizeof(producto.descripcion), stdin);
    producto.descripcion[strcspn(producto.descripcion, "\n")] = '\0'; 
    
    printf("Ingrese el precio del producto: ");
    scanf("%f", &producto.precio);
    getchar(); 

    printf("Ingrese el peso del producto: ");
    scanf("%f", &producto.peso);
    getchar(); 
    
    printf("Ingrese las calorias del producto: ");
    scanf("%d", &producto.calorias);
    getchar(); 
    
    printf("Ingrese la cantidad disponible del producto: ");
    scanf("%d", &producto.cantidad);
    getchar(); 
    
    productos[*numProductos] = producto;
    (*numProductos)++;
    
    printf("Producto agregado correctamente.\n\n");
}

void mostrarContabilidad(struct Producto productos[], int numProductos) {
    int cantidadVendida = 0;
    float montoTotal = 0;
    
    for (int i = 0; i < numProductos; i++) {
        cantidadVendida += productos[i].cantidad - 1;
        montoTotal += productos[i].precio * (productos[i].cantidad - 1);
    }
    
    printf("Cantidad de productos restantes: %d\n", cantidadVendida);
    printf("Monto total invertido en productos: %.2f\n\n", montoTotal);
}

void mostrarProductos(struct Producto productos[], int numProductos) {
    printf("---- Productos Disponibles ----\n");
    
    for (int i = 0; i < numProductos; i++) {
        printf("Nombre: %s\n", productos[i].nombre);
        printf("Descripcion: %s\n", productos[i].descripcion);
        printf("Precio: %.2f\n", productos[i].precio);
        printf("Peso: %.2f\n", productos[i].peso);
        printf("Calorias: %d\n", productos[i].calorias);
        printf("Cantidad: %d\n\n", productos[i].cantidad);
    }
}

void realizarVenta(struct Producto productos[], int numProductos) {
    if (numProductos == 0) {
        printf("No hay productos disponibles para la venta.\n\n");
        return;
    }

    int opcion;
    int cantidad;
    float total = 0;

    printf("---- Realizar Venta ----\n");
    mostrarProductos(productos, numProductos);

    printf("Seleccione el producto a vender (1-%d): ", numProductos);
    scanf("%d", &opcion);

    if (opcion < 1 || opcion > numProductos) {
        printf("Opcion invalida.\n\n");
        return;
    }

    printf("Ingrese la cantidad a vender: ");
    scanf("%d", &cantidad);

    if (cantidad <= 0 || cantidad > productos[opcion - 1].cantidad) {
        printf("Cantidad invalida.\n\n");
        return;
    }

    total = cantidad * productos[opcion - 1].precio;

    printf("Total a pagar: %.2f\n", total);

    // Proceso de pago
    float pago;
    printf("Ingrese el monto recibido (billetes de 100 y 50): ");
    scanf("%f", &pago);

    if (pago < total) {
        printf("Monto insuficiente.\n\n");
        return;
    }

    // Calcular cambio
    float cambio = pago - total;
    printf("Cambio a devolver: %.2f\n", cambio);

    // Actualizar la cantidad de productos después de la venta
    productos[opcion - 1].cantidad -= cantidad;

    printf("Venta realizada exitosamente.\n\n");
}

void guardarProductos(struct Producto productos[], int numProductos) {
    FILE* archivo = fopen("productos.txt", "w");
    
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de productos.\n\n");
        return;
    }
    
    for (int i = 0; i < numProductos; i++) {
        fprintf(archivo, "Nombre: %s\n", productos[i].nombre);
        fprintf(archivo, "Descripcion: %s\n", productos[i].descripcion);
        fprintf(archivo, "Precio: %.2f\n", productos[i].precio);
        fprintf(archivo, "Peso: %.2f\n", productos[i].peso);
        fprintf(archivo, "Calorias: %d\n", productos[i].calorias);
        fprintf(archivo, "Cantidad: %d\n\n", productos[i].cantidad);
    }
    
    fclose(archivo);
}

void cargarProductos(struct Producto productos[], int* numProductos) {
    FILE* archivo = fopen("productos.txt", "r");
    
    if (archivo == NULL) {
        return;
    }
    
    char linea[200];
    while (fgets(linea, sizeof(linea), archivo)) {
        sscanf(linea, "Nombre: %[^\n]", productos[*numProductos].nombre);
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "Descripcion: %[^\n]", productos[*numProductos].descripcion);
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "Precio: %f", &productos[*numProductos].precio);
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "Peso: %f", &productos[*numProductos].peso);
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "Calorias: %d", &productos[*numProductos].calorias);
        fgets(linea, sizeof(linea), archivo);
        sscanf(linea, "Cantidad: %d", &productos[*numProductos].cantidad);
        fgets(linea, sizeof(linea), archivo);
        
        (*numProductos)++;
    }
    
    fclose(archivo);
}
