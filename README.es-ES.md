<div align="center">

# 🎵 Cliente de Música Estilo KuGou - Redefiniendo la Experiencia Musical con Qt😌

</div>

<p align="center">
  <img src="https://img.shields.io/badge/Qt%20Music%20Client-v1.0.0-%23007ACC?logo=qt" />
  <img src="https://img.shields.io/badge/Qt-6.6.2-%2341CD52?logo=qt" />
  <img src="https://gitee.com/a-mo-xi-wei/KuGouApp/badge/star.svg?theme=red" />
  <br>
  <span>🔥</span>
  <img src="https://img.shields.io/badge/CLion-2023.3-%23000000?logo=clion" />
  <img src="https://img.shields.io/badge/C%2B%2B-17-%2300599C?logo=c%2B%2B" />
<img src="https://img.shields.io/badge/MinGW-Native-A42E2B?logo=gnu" />
  <span>🔥</span>
</p>

<p align="center">
🎁 Si te gusta este proyecto, por favor haz clic en la ✨estrella✨ arriba a la derecha. Tu <strong>Star</strong> es mi mayor motivación.
<br>
<strong>El código de este proyecto es solo para fines de estudio e investigación; los derechos de autor de los recursos pertenecen a las plataformas musicales.</strong>
</p>

---

### Para el desarrollo utilicé CLion, que es muy sencillo e intuitivo. Para la compilación, el generador utilizado es Ninja y la cadena de herramientas es **_MinGW_**; por favor, no use VS2022 (MSVC). Si selecciona el tipo de construcción Release (recomendado), la compilación será mucho más rápida. Los usuarios de Windows 10/11 pueden elegir la rama master (más funciones) que está en desarrollo continuo y no garantiza estabilidad 😜, o la versión Stable (más estable, funciones limitadas y ya no recibe mantenimiento).

![Configuración como se muestra en la imagen](image-show/settings00.png)
![Configuración como se muestra en la imagen](image-show/settings01.png)

Si aparece un error similar al siguiente en la rama de Ubuntu:

```
/home/xxx/Desktop/KuGou/tools/ccache/ccache: error while loading shared libraries: libhiredis.so.0.14: cannot open shared object file: No such file or directory
```

Por favor, ejecute los siguientes comandos:

```
sudo apt install ccache
rm YourProjectPath/KuGou/tools/ccache/ccache
cp /usr/bin/ccache YourProjectPath/KuGou/tools/ccache
```

#### Al mismo tiempo, aunque la compilación en Ubuntu es exitosa y el programa se ejecuta, existen problemas con la recepción de eventos del ratón y el renderizado de estilos. Debido a que mis recursos son limitados, no daré soporte a esta plataforma...

### La función de chat de IA requiere obtener una API previamente. Para más información, consulte: https://gitee.com/a-mo-xi-wei/qt6-access-deepseek#qt6-%E6%8E%A5%E5%85%A5deepseek

| [Ubuntu/macOS][lin-link] | [Windows][win-link] | [Licencia][license-link] |
|:------------------------:|:-------------------:|:-----------------------:|
|       ![lin-badge]       |    ![win-badge]     |    ![license-badge]     |

[lin-link]: https://你的Ubuntu/macOS构建状态链接

[win-link]: https://你的Windows构建状态链接

[license-link]: https://你的License文件链接

[lin-badge]: https://img.shields.io/badge/Ubuntu/macOS-Passing-lightgrey?logo=linux

[win-badge]: https://img.shields.io/badge/Windows-Passing-success?logo=windows

[license-badge]: https://img.shields.io/badge/License-MIT-blue.svg

### 📚 Introducción del Proyecto

Basado en Qt Widget + Módulo UiTools + Controles personalizados + QSS + QsLog + **Spdlog** personalizado + **JWT** + **FFmpeg** + **SDL**.

Utiliza **CMake** para la gestión modular, asegurando un **alto desacoplamiento** de los componentes principales (como la interfaz de usuario, el reproductor de medios y la capa de red). Basado en el cliente de KuGou, el diseño ha sido imitado y optimizado.

La aplicación emplea actualmente **FFmpeg** y **SDL** para la decodificación y reproducción de audio, proporcionando una experiencia musical más flexible y potente.

Integra **QsLog** y un **Spdlog** personalizado para implementar un sistema de registro (logging) robusto, utilizando JWT para garantizar una autenticación de usuario segura. Utiliza **SQLite** para almacenar metadatos musicales (título/álbum/ruta) y el módulo Qt SQL para gestionar el historial de reproducción.

A través de **QNetworkAccessManager** se implementa la reproducción de música vía streaming en línea, combinando QtConcurrent y QFutureWatcher para la programación de tareas seriales en hilos no bloqueantes. Se construye un pool de hilos dinámico y elástico basado en **QThreadPool**, encapsulando las unidades de tarea mediante QRunnable para lograr la recuperación automática de recursos de hilos.

Dificultades del proyecto: Diseño de las funciones principales del reproductor como la lista de reproducción, menús, **búsqueda de canciones**, **visualización de letras** y controles de reproducción; diseño de controles personalizados y respuesta a eventos, dibujo de máscaras, estiramiento de controles, **renderizado de efectos especiales**, **carga perezosa (lazy loading)** de interfaces apiladas, **caché LRU**, **autenticación JWT**, técnicas de **pooling**, **chat de IA**, **hilos asíncronos no bloqueantes**, **sistema de logs**, encapsulamiento del **módulo de red** e integración y optimización de **librerías de código abierto** (como FFmpeg y SDL) para mejorar la capacidad de reproducción de medios.

## 🎨 Stack Tecnológico de la Interfaz

Magia Visual: Motor de estilos QSS + Iconos vectoriales SVG + Máscaras de degradado dinámico.

Tecnología de Interacción: Sistema de animación de propiedades + Adaptación de diseño inteligente.

Garantía de Rendimiento: Dibujo de doble búfer + Carga asíncrona + Estrategias de refresco parcial.

✅ Soporte directo para formatos sin pérdida (lossless).

✅ Ecualizador visual de 32 bandas.

✅ Tecnología de compensación de sonoridad dinámica.

### 🔌 Arquitectura Central

```mermaid
flowchart TD
%% 模块定义
    subgraph 构建系统
        CMakeModule["CMake模块化构建"]
    end

    subgraph 表现层
        UILogic["UI逻辑层"]
        CustomWidgets["自定义控件库"]
    end

    subgraph 核心逻辑层
        CoreCode["核心代码层"]
        KuGouClient["KuGouClient"]
        KuGouServer["KuGouServer"]
    end

    subgraph 第三方依赖层
        ThirdParty["第三方管理层"]
        PoolAsyncNetwork["池化 异步 网络"]
    end

%% 连接关系
    CMakeModule -->|"数据/控制流"| UILogic
    CMakeModule -->|"数据/控制流"| CoreCode
    CMakeModule -->|"数据/控制流"| ThirdParty

    UILogic -->|"调用/交互"| CustomWidgets
    CoreCode -->|"业务逻辑调用"| KuGouClient
    CoreCode -->|"业务逻辑调用"| KuGouServer

    ThirdParty -->|"封装/使用"| PoolAsyncNetwork

    CustomWidgets -- UI事件回传 --> UILogic
    KuGouClient -- 网络请求 --> PoolAsyncNetwork
    KuGouServer -- 网络请求 --> PoolAsyncNetwork

```

<!-- ![核心架构](image-show/architecture.png)] -->

## 🛠️ Panorama Técnico

### 0. Pantalla de Inicio de Sesión y Registro

![Descripción de la imagen](image-show/image00.png)
![Descripción de la imagen](image-show/image00-1.png)

### 1. Pantalla de Recomendaciones para ti

![Descripción de la imagen](image-show/image01.png)
![Descripción de la imagen](image-show/image01-1.png)
![Descripción de la imagen](image-show/image01-2.png)
![Descripción de la imagen](image-show/image01-3.png)
![Descripción de la imagen](image-show/image01-4.png)
![Descripción de la imagen](image-show/image01-5.png)
![Descripción de la imagen](image-show/image01-6.png)
![Descripción de la imagen](image-show/image01-7.png)
![Descripción de la imagen](image-show/image02.png)
![Descripción de la imagen](image-show/image03.png)

### 2. Pantalla de Biblioteca Musical

![Descripción de la imagen](image-show/image04.png)
![Descripción de la imagen](image-show/image05.png)

### 3. Pantalla de Canales

![Descripción de la imagen](image-show/image06.png)

### 4. Pantalla de Video

![Descripción de la imagen](image-show/image07.png)
![Descripción de la imagen](image-show/image08.png)
![Descripción de la imagen](image-show/image09.png)

### 5. Pantalla de Chat de IA (un poco lenta 😅)

![](image-show/image24.png)
![](image-show/image25.png)

### 6. Pantalla de Listas de Reproducción

![Descripción de la imagen](image-show/image10.png)

### 7. Pantalla de Recomendaciones Diarias

![Descripción de la imagen](image-show/image11.png)

### 8. Pantalla de Mis Colecciones

![Descripción de la imagen](image-show/image12.png)

### 9. Pantalla de Local y Descargas

![Descripción de la imagen](image-show/image13.png)
![Descripción de la imagen](image-show/image14.png)

### 10. Pantalla de Disco en la Nube

![Descripción de la imagen](image-show/image15.png)

### 11. Pantalla de Música Comprada

![Descripción de la imagen](image-show/image16.png)

### 12. Pantalla de Reproducciones Recientes

![Descripción de la imagen](image-show/image17.png)

### 13. Pantalla de Toda la Música

![Descripción de la imagen](image-show/image18.png)

### 14. Pantalla de Exploración

![Descripción de la imagen](image-show/image19.png)

### 15. Pantalla de Audiolibros

![Descripción de la imagen](image-show/image20.png)
![Descripción de la imagen](image-show/image21.png)

### 16. Pantalla de Transmisiones en Vivo

![Descripción de la imagen](image-show/image22.png)
![Descripción de la imagen](image-show/image23.png)

### 17. Pantalla de Búsqueda de Canciones en Red

![Descripción de la imagen](image-show/image26.png)
![Descripción de la imagen](image-show/image27.png)

---

🌈 Hoja de Ruta Futura

🛸 Reconocimiento de Huella Vocal AI: Solicitud de canciones por voz + reconocimiento de cantantes.

🌐 Intercambio de Música P2P: Red musical distribuida.

🔮 Modo Concierto VR: Espacialización de audio 3D.

🤖 Motor de Recomendación Inteligente: Algoritmos de recomendación basados en redes neuronales.

🔮 Sistema de Letras Mágicas

    Animaciones de flujo luminoso: Efecto de desvanecimiento de píxeles similar a pantallas OLED.
     
    Modo Karaoke: Coloreado palabra por palabra + sincronización de forma de onda.
     
    Bilingüismo: Alineación inteligente de la línea de tiempo de letras en dos idiomas.

---

## 😊 Donaciones

<div style="display: flex; justify-content: flex-start; gap: 2cm;">
  <img src="image-show/reward_1.png" width="300"/>
  <img src="image-show/reward_2.png" width="300"/>
</div>
 
### Si este proyecto te ha resultado útil, ¡puedes invitarme a una Coca-Cola fría!~~
