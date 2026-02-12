# Creative-HUB-System

Creative HUB System es una estación de control táctil personalizada, diseñada para optimizar flujos de trabajo en producción audiovisual y diseño gráfico. Utiliza un ESP32-S3 para gestionar una interfaz de usuario avanzada que permite el control directo de herramientas como After Effects y Premiere Pro.

Este proyecto nace de la necesidad de centralizar macros, automatizaciones y scripts creativos en un dispositivo físico dedicado (estilo Stream Deck), aprovechando la potencia de procesamiento gráfico del microcontrolador S3.

🛠️ Hardware Stack
Microcontrolador: ESP32-S3 (Dual-core con instrucciones vectoriales para aceleración de IA y gráficos).

Pantalla: Panel táctil de 7 pulgadas (HMI) para una interacción fluida y visualización de estados en tiempo real.

Conectividad: USB/Serial y Wi-Fi para comunicación con la suite de Adobe.

✨ Características Principales
Control de Adobe Creative Cloud: Integración de macros específicas para agilizar procesos de edición y animación.

Interfaz HMI Avanzada: Diseñada para ser intuitiva, permitiendo el lanzamiento de scripts complejos (como la separación de texto en capas en After Effects) con un solo toque.

Arquitectura Escalable: Preparado para integrar futuros módulos de automatización y asistentes de IA locales.

📂 Estructura del Repositorio
/Firmware: Código fuente del ESP32-S3 y gestión de la pantalla táctil.

/Scripts: Automatizaciones en ExtendScript para After Effects y Premiere.

/Hardware: Documentación técnica, dimensiones y esquemas del display.

/Assets: Iconografía y recursos gráficos de la interfaz.

🔒 Licencia y Propiedad Intelectual
© 2026 Ricardo González. Todos los derechos reservados.

Este código y diseño son propiedad privada. No se autoriza la reproducción, distribución o modificación de este proyecto sin el consentimiento explícito del autor.
