const section = document.querySelector('.section1');
const gradient = section.querySelector('::before');

document.addEventListener('mousemove', (e) => {
    const { clientX, clientY } = e; // Get cursor position
    const { innerWidth, innerHeight } = window; // Get window size

    // Calculate gradient movement relative to cursor position
    const x = ((clientX / innerWidth) - 0.5) * 100;
    const y = ((clientY / innerHeight) - 0.5) * 100;

    // Update `::before` pseudo-element using CSS custom properties
    section.style.setProperty('--gradient-x', `${x}%`);
    section.style.setProperty('--gradient-y', `${y}%`);
});