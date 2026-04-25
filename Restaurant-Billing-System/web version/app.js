// --- State Management & Initialization ---
const DEFAULT_MENU = [
    { id: 1, name: "Burger", price: 100, type: "f", category: "veg" },
    { id: 2, name: "Pizza", price: 250, type: "f", category: "veg" },
    { id: 3, name: "Chicken Roll", price: 180, type: "f", category: "non-veg" },
    { id: 4, name: "Coke", price: 50, type: "d", category: "drink" },
    { id: 5, name: "Coffee", price: 80, type: "d", category: "drink" }
];

let menuItems = JSON.parse(localStorage.getItem('savoria_menu')) || DEFAULT_MENU;
let orders = JSON.parse(localStorage.getItem('savoria_orders')) || [];
let currentCustomer = null;
let cart = [];

// Ensure default menu exists if local storage is completely empty
if (!localStorage.getItem('savoria_menu')) {
    saveMenu();
}

function saveMenu() {
    localStorage.setItem('savoria_menu', JSON.stringify(menuItems));
}

function saveOrders() {
    localStorage.setItem('savoria_orders', JSON.stringify(orders));
}

// --- DOM Elements ---
const views = {
    landing: document.getElementById('view-landing'),
    admin: document.getElementById('view-admin'),
    customerOrder: document.getElementById('view-customer-order')
};

const modals = {
    invoice: document.getElementById('modal-invoice'),
    editItem: document.getElementById('modal-edit-item'),
    findBill: document.getElementById('modal-find-bill')
};

// --- View Navigation ---
function switchView(viewName) {
    Object.values(views).forEach(v => v.classList.remove('active'));
    views[viewName].classList.add('active');
}

function showModal(modalName) {
    modals[modalName].classList.remove('hidden');
}

function hideModal(modalName) {
    modals[modalName].classList.add('hidden');
}

function showToast(message, type = 'success') {
    const container = document.getElementById('toast-container');
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    const iconName = type === 'success' ? 'checkmark-circle-outline' : 'warning-outline';
    toast.innerHTML = `<ion-icon name="${iconName}"></ion-icon> <span>${message}</span>`;
    container.appendChild(toast);
    setTimeout(() => toast.remove(), 3000);
}

// --- Unified Login Flow ---
document.querySelector('#modal-edit-item .close-modal').addEventListener('click', () => hideModal('editItem'));
document.querySelector('#modal-find-bill .close-modal').addEventListener('click', () => hideModal('findBill'));

document.getElementById('btn-find-bill').addEventListener('click', () => {
    showModal('findBill');
});

document.getElementById('form-find-bill').addEventListener('submit', (e) => {
    e.preventDefault();
    const query = document.getElementById('find-bill-query').value.trim();
    
    // Search orders by mobile or ID (find the most recent one if multiple)
    const foundOrder = [...orders].reverse().find(o => o.id.toString() === query || o.customerMobile === query);
    
    if (foundOrder) {
        hideModal('findBill');
        viewHistoricalOrder(foundOrder.id);
        document.getElementById('form-find-bill').reset();
    } else {
        showToast("No order found with that ID or Mobile No.", "error");
    }
});

document.getElementById('unified-login-form').addEventListener('submit', (e) => {
    e.preventDefault();
    const loginId = document.getElementById('login-id').value.trim();
    const loginPass = document.getElementById('login-pass').value.trim();
    
    // Check if it's admin
    if (loginId.toLowerCase() === 'admin') {
        if (loginPass === '2004') {
            switchView('admin');
            document.getElementById('unified-login-form').reset();
            renderAdminMenu();
            showToast("Admin access granted.");
        } else {
            showToast("Access denied. Invalid admin PIN.", "error");
        }
    } else {
        // Customer login
        const mobileRegex = /^[0-9]{10}$/;
        if (!mobileRegex.test(loginPass)) {
            showToast("Customers must enter a valid 10-digit mobile number.", "error");
            return;
        }
        
        currentCustomer = {
            id: Math.floor(Math.random() * 9000) + 1000,
            name: loginId,
            mobile: loginPass
        };
        
        document.getElementById('display-cust-info').innerHTML = `
            <strong>${currentCustomer.name}</strong><br>
            +91 ${currentCustomer.mobile}
        `;
        
        document.getElementById('unified-login-form').reset();
        switchView('customerOrder');
        renderCustomerMenu('all');
        showToast(`Welcome, ${currentCustomer.name}!`);
    }
});

document.getElementById('btn-admin-logout').addEventListener('click', () => {
    switchView('landing');
    showToast("Logged out successfully.");
});

// --- Admin Navigation ---
document.querySelectorAll('.nav-links li').forEach(li => {
    li.addEventListener('click', (e) => {
        document.querySelectorAll('.nav-links li').forEach(el => el.classList.remove('active'));
        e.currentTarget.classList.add('active');
        const target = e.currentTarget.getAttribute('data-target');

        document.querySelectorAll('.admin-section').forEach(sec => {
            sec.classList.remove('active');
            sec.classList.add('hidden');
        });
        const targetSection = document.getElementById(target);
        targetSection.classList.remove('hidden');
        targetSection.classList.add('active');
        
        if (target === 'admin-orders') {
            renderAdminOrders();
        }
    });
});

// --- Admin Menu CRUD ---
function renderAdminMenu() {
    const tbody = document.querySelector('#admin-menu-table tbody');
    tbody.innerHTML = '';
    document.getElementById('stat-total-items').textContent = `${menuItems.length} Items`;

    menuItems.forEach(item => {
        const typeLabel = item.type === 'f' ? 'Food' : 'Drink';
        const typeClass = item.type === 'f' ? 'food' : 'drink';
        const categoryLabel = item.category || '-';

        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td>#${item.id}</td>
            <td><strong>${item.name}</strong></td>
            <td><span class="badge ${typeClass}">${typeLabel}</span></td>
            <td style="text-transform: capitalize;">${categoryLabel}</td>
            <td>₹${item.price.toFixed(2)}</td>
            <td class="action-btns">
                <button class="icon-btn edit" onclick="editMenuItem(${item.id})">
                    <ion-icon name="pencil-outline"></ion-icon>
                </button>
                <button class="icon-btn delete" onclick="deleteMenuItem(${item.id})">
                    <ion-icon name="trash-outline"></ion-icon>
                </button>
            </td>
        `;
        tbody.appendChild(tr);
    });
}

function deleteMenuItem(id) {
    if (confirm("Are you sure you want to delete this item?")) {
        menuItems = menuItems.filter(i => i.id !== id);
        saveMenu();
        renderAdminMenu();
        showToast("Item deleted.");
    }
}

// Form dynamic category logic
document.getElementById('add-type').addEventListener('change', (e) => {
    const catSelect = document.getElementById('add-category');
    if (e.target.value === 'd') {
        catSelect.value = 'drink';
        catSelect.disabled = true;
    } else {
        catSelect.disabled = false;
        catSelect.value = 'veg'; // default to veg
    }
});

document.getElementById('form-add-item').addEventListener('submit', (e) => {
    e.preventDefault();
    const type = document.getElementById('add-type').value;
    const isDrink = type === 'd';
    const category = isDrink ? 'drink' : document.getElementById('add-category').value;

    const newItem = {
        id: menuItems.length > 0 ? Math.max(...menuItems.map(i => i.id)) + 1 : 1,
        name: document.getElementById('add-name').value,
        price: parseFloat(document.getElementById('add-price').value),
        type: type,
        category: category
    };

    menuItems.push(newItem);
    saveMenu();
    document.getElementById('form-add-item').reset();
    document.getElementById('add-category').disabled = false;
    renderAdminMenu();
    showToast("Item added successfully.");
});

function editMenuItem(id) {
    const item = menuItems.find(i => i.id === id);
    if (!item) return;

    document.getElementById('edit-id').value = item.id;
    document.getElementById('edit-name').value = item.name;
    document.getElementById('edit-price').value = item.price;
    document.getElementById('edit-type').value = item.type;

    const catSelect = document.getElementById('edit-category');
    if (item.type === 'd') {
        catSelect.value = 'drink';
        catSelect.disabled = true;
    } else {
        catSelect.disabled = false;
        catSelect.value = item.category;
    }

    showModal('editItem');
}

document.getElementById('edit-type').addEventListener('change', (e) => {
    const catSelect = document.getElementById('edit-category');
    if (e.target.value === 'd') {
        catSelect.value = 'drink';
        catSelect.disabled = true;
    } else {
        catSelect.disabled = false;
        catSelect.value = 'veg'; // default
    }
});

document.getElementById('form-edit-item').addEventListener('submit', (e) => {
    e.preventDefault();
    const id = parseInt(document.getElementById('edit-id').value);
    const type = document.getElementById('edit-type').value;
    const category = type === 'd' ? 'drink' : document.getElementById('edit-category').value;

    const index = menuItems.findIndex(i => i.id === id);
    if (index > -1) {
        menuItems[index] = {
            ...menuItems[index],
            name: document.getElementById('edit-name').value,
            price: parseFloat(document.getElementById('edit-price').value),
            type: type,
            category: category
        };
        saveMenu();
        renderAdminMenu();
        hideModal('editItem');
        showToast("Item updated successfully.");
    }
});

// System Operations
document.getElementById('btn-clear-menu').addEventListener('click', () => {
    if (confirm("WARNING: This will delete ALL menu items. Are you sure?")) {
        menuItems = [];
        saveMenu();
        renderAdminMenu();
        showToast("All items cleared.", "error");
    }
});

document.getElementById('btn-reset-default').addEventListener('click', () => {
    if (confirm("Restore default menu? This replaces current items.")) {
        menuItems = [...DEFAULT_MENU];
        saveMenu();
        renderAdminMenu();
        showToast("Default menu restored.");
    }
});

function renderAdminOrders() {
    const tbody = document.querySelector('#admin-orders-table tbody');
    tbody.innerHTML = '';
    
    // Sort by newest first
    const sortedOrders = [...orders].reverse();
    
    if (sortedOrders.length === 0) {
        tbody.innerHTML = `<tr><td colspan="6" class="text-center text-muted">No orders found.</td></tr>`;
        return;
    }
    
    sortedOrders.forEach(order => {
        const tr = document.createElement('tr');
        const dateObj = new Date(order.timestamp);
        const dateString = `${dateObj.toLocaleDateString()} ${dateObj.toLocaleTimeString()}`;
        
        tr.innerHTML = `
            <td>#${order.id}</td>
            <td><strong>${order.customerName}</strong></td>
            <td>${order.customerMobile}</td>
            <td>${dateString}</td>
            <td><strong>₹${order.total.toFixed(2)}</strong></td>
            <td class="action-btns" style="display: flex; gap: 5px; align-items: center; justify-content: center;">
                <button class="btn secondary-btn" style="padding: 6px 12px; font-size: 0.85rem;" onclick="viewHistoricalOrder(${order.id})">
                    <ion-icon name="eye-outline"></ion-icon> View Bill
                </button>
                <button class="icon-btn delete" onclick="deleteHistoricalOrder(${order.id})" title="Delete Bill">
                    <ion-icon name="trash-outline"></ion-icon>
                </button>
            </td>
        `;
        tbody.appendChild(tr);
    });
}

// --- Customer Logout Logic ---
document.getElementById('btn-customer-logout').addEventListener('click', () => {
    if (confirm("Are you sure you want to cancel this order and log out?")) {
        cart = [];
        currentCustomer = null;
        renderCart();
        switchView('landing');
        showToast("Logged out successfully.");
    }
});

// --- Customer Order Flow ---
document.querySelectorAll('.filter-btn').forEach(btn => {
    btn.addEventListener('click', (e) => {
        document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
        e.target.classList.add('active');
        renderCustomerMenu(e.target.getAttribute('data-filter'));
    });
});

function renderCustomerMenu(filter) {
    const grid = document.getElementById('customer-menu-grid');
    grid.innerHTML = '';

    let filteredItems = menuItems;
    if (filter !== 'all') {
        filteredItems = menuItems.filter(item => item.category === filter);
    }

    if (filteredItems.length === 0) {
        grid.innerHTML = `<div class="w-100 text-center text-muted"><p>No items found.</p></div>`;
        return;
    }

    filteredItems.forEach(item => {
        const icon = item.type === 'd' ? 'wine-outline' : (item.category === 'veg' ? 'leaf-outline' : 'restaurant-outline');
        const color = item.category === 'veg' ? 'var(--success)' : (item.category === 'drink' ? 'var(--secondary)' : 'var(--danger)');

        const card = document.createElement('div');
        card.className = 'menu-item-card glass-panel';
        card.innerHTML = `
            <ion-icon name="${icon}" class="menu-item-icon" style="color: ${color}"></ion-icon>
            <h3 class="menu-item-name">${item.name}</h3>
            <p class="text-muted" style="text-transform:capitalize;font-size:0.8rem;margin-bottom:0.5rem">${item.category}</p>
            <p class="menu-item-price">₹${item.price.toFixed(2)}</p>
            <button class="btn secondary-btn add-to-cart-btn" onclick="addToCart(${item.id})">Add to Order</button>
        `;
        grid.appendChild(card);
    });
}

function addToCart(id) {
    const item = menuItems.find(i => i.id === id);
    if (!item) return;

    const existing = cart.find(c => c.id === id);
    if (existing) {
        existing.qty += 1;
    } else {
        cart.push({ ...item, qty: 1 });
    }
    renderCart();
}

function updateQty(id, delta) {
    const itemIndex = cart.findIndex(c => c.id === id);
    if (itemIndex > -1) {
        cart[itemIndex].qty += delta;
        if (cart[itemIndex].qty <= 0) {
            cart.splice(itemIndex, 1);
        }
        renderCart();
    }
}

function renderCart() {
    const cartContainer = document.getElementById('cart-container');
    const checkoutBtn = document.getElementById('btn-checkout');
    cartContainer.innerHTML = '';

    if (cart.length === 0) {
        cartContainer.innerHTML = `
            <div class="empty-cart">
                <ion-icon name="cart-outline"></ion-icon>
                <p>Your cart is empty</p>
            </div>
        `;
        checkoutBtn.disabled = true;
        document.getElementById('cart-subtotal').textContent = '₹0.00';
        document.getElementById('cart-gst').textContent = '₹0.00';
        document.getElementById('cart-st').textContent = '₹0.00';
        document.getElementById('cart-total').textContent = '₹0.00';
        return;
    }

    let subtotal = 0;

    cart.forEach(item => {
        const itemTotal = item.price * item.qty;
        subtotal += itemTotal;

        const div = document.createElement('div');
        div.className = 'cart-item';
        div.innerHTML = `
            <div class="cart-item-info">
                <h4>${item.name}</h4>
                <p>₹${item.price.toFixed(2)}</p>
            </div>
            <div class="qty-controls">
                <button class="qty-btn" onclick="updateQty(${item.id}, -1)"><ion-icon name="remove-circle-outline"></ion-icon></button>
                <span class="qty-val">${item.qty}</span>
                <button class="qty-btn" onclick="updateQty(${item.id}, 1)"><ion-icon name="add-circle-outline"></ion-icon></button>
            </div>
        `;
        cartContainer.appendChild(div);
    });

    const gst = subtotal * 0.12;
    const st = subtotal * 0.06;
    const total = subtotal + gst + st;

    document.getElementById('cart-subtotal').textContent = `₹${subtotal.toFixed(2)}`;
    document.getElementById('cart-gst').textContent = `₹${gst.toFixed(2)}`;
    document.getElementById('cart-st').textContent = `₹${st.toFixed(2)}`;
    document.getElementById('cart-total').textContent = `₹${total.toFixed(2)}`;
    checkoutBtn.disabled = false;
}

// --- Checkout & Invoice ---
function createOrderDataFromCart() {
    let subtotal = 0;
    cart.forEach(item => subtotal += item.price * item.qty);
    const gst = subtotal * 0.12;
    const st = subtotal * 0.06;
    const grandTotal = subtotal + gst + st;
    
    return {
        id: Math.floor(Math.random() * 90000) + 10000,
        timestamp: new Date().toISOString(),
        customerName: currentCustomer.name,
        customerMobile: currentCustomer.mobile,
        items: JSON.parse(JSON.stringify(cart)),
        subtotal, gst, st, total: grandTotal
    };
}

let activeOrderCache = null;

document.getElementById('btn-checkout').addEventListener('click', () => {
    activeOrderCache = createOrderDataFromCart();
    populateInvoiceModal(activeOrderCache);
    document.getElementById('btn-close-invoice').style.display = 'block';
    showModal('invoice');
});

function populateInvoiceModal(order) {
    const tbody = document.getElementById('invoice-items');
    tbody.innerHTML = '';
    
    order.items.forEach(item => {
        const itemTotal = item.price * item.qty;
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td>${item.name}</td>
            <td>${item.qty}</td>
            <td>₹${itemTotal.toFixed(2)}</td>
        `;
        tbody.appendChild(tr);
    });
    
    const dateObj = new Date(order.timestamp);
    
    document.getElementById('invoice-details').innerHTML = `
        <p><strong>Order ID:</strong> #${order.id}</p>
        <p><strong>Name:</strong> ${order.customerName}</p>
        <p><strong>Mobile:</strong> ${order.customerMobile}</p>
        <p><strong>Date:</strong> ${dateObj.toLocaleDateString()} ${dateObj.toLocaleTimeString()}</p>
    `;
    
    document.getElementById('inv-subtotal').textContent = `₹${order.subtotal.toFixed(2)}`;
    document.getElementById('inv-gst').textContent = `₹${order.gst.toFixed(2)}`;
    document.getElementById('inv-st').textContent = `₹${order.st.toFixed(2)}`;
    document.getElementById('inv-total').textContent = `₹${order.total.toFixed(2)}`;
}

document.getElementById('btn-close-invoice').addEventListener('click', () => {
    if (activeOrderCache) {
        orders.push(activeOrderCache);
        saveOrders();
        activeOrderCache = null;
    }
    
    cart = [];
    currentCustomer = null;
    renderCart();
    hideModal('invoice');
    switchView('landing');
    showToast("Order completed successfully.");
});

function deleteHistoricalOrder(orderId) {
    if (confirm("Are you sure you want to delete this bill? This action cannot be undone.")) {
        orders = orders.filter(o => o.id !== orderId);
        saveOrders();
        renderAdminOrders();
        showToast("Bill deleted successfully.");
    }
}

function viewHistoricalOrder(orderId) {
    const order = orders.find(o => o.id === orderId);
    if (!order) return;
    populateInvoiceModal(order);
    document.getElementById('btn-close-invoice').style.display = 'none';
    showModal('invoice');
}

document.getElementById('btn-print-invoice').addEventListener('click', () => {
    window.print();
});

// Initialize
switchView('landing');
