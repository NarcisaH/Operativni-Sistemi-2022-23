# Use an official Nginx runtime as a parent image
FROM nginx:latest

# Set the working directory to /var/www/html
WORKDIR /var/www/html

# Copy the PHP files and configuration file into the container
COPY index.php /var/www/html/
COPY config.php /var/www/html/

# Install PHP and the MySQL driver
RUN apt-get update && \
    apt-get install -y php-fpm php-mysql default-mysql-client

# Copy the Nginx configuration file
COPY default.conf /etc/nginx/conf.d/

# Expose port 80 for Nginx
EXPOSE 80
