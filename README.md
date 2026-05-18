# Restaurant Order System

This project contains two C++ applications that work together using a shared text file:

- **Customer app**: creates and views customer orders.
- **Kitchen app**: views orders and updates their status.

Both apps read/write to `orders.txt`, so they stay in sync through the same order data.

## What The Customer App Does

The customer side is used to submit new orders.

### GUI mode
When started without CLI arguments, the app opens an ImGui window where you can:

- Enter an order name (product).
- Click **Place Order** to add a new order.
- Click **Reload Orders** to refresh from disk.
- View all current orders with `ID | Product | Status`.

New orders are created with:

- Auto-incremented order ID (next available ID from file).
- Default status: `Pending`.

### CLI mode
If arguments are provided, the customer app runs in command mode:

- `customer.exe view`
  - Shows all current orders.
- `customer.exe place <PRODUCT>`
  - Adds a new order with status `Pending`.

Validation behavior:

- Rejects unknown commands.
- Rejects missing/extra arguments.
- Rejects empty product input.

## What The Kitchen App Does

The kitchen side is used to monitor and update orders.

### GUI mode
When started without CLI arguments, the app opens an ImGui window where you can:

- Click **Reload Orders** to refresh the order list.
- View orders in a table (`ID`, `Product`, `Status`).
- Enter an order ID and a new status.
- Click **Update Status** to change and save that order.

Example statuses might be: `Preparing`, `Ready`, `Served`.

### CLI mode
If arguments are provided, the kitchen app runs in command mode:

- `kitchen.exe view`
  - Shows all orders.
- `kitchen.exe update <ID> <STATUS>`
  - Updates one order by ID and saves all orders back to file.

Validation behavior:

- Rejects non-numeric or non-positive IDs.
- Rejects unknown commands.
- Rejects missing/extra arguments.
- Returns an error if the order ID does not exist.

## Shared Data File

The apps communicate through:

- `orders.txt`

Each line is stored in this format:

- `id|product|status`

Example:

- `1|Burger|Pending`
- `2|Pasta|Ready`

## Typical Workflow

1. Customer places an order (status starts as `Pending`).
2. Kitchen reloads and sees the new order.
3. Kitchen updates status as work progresses.
4. Customer reloads to see updated statuses.

## Notes

- The GUI is built with Dear ImGui + DirectX 11 on Windows.
- If `orders.txt` is missing, customer file loading logic can create it when needed.
