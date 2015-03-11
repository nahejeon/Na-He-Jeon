class AddLoginToUser < ActiveRecord::Migration
  def change
  	add_column :users, :login, :string
  	User.reset_column_information
  	User.find_each do |user|
  		user[:login] = user.last_name.downcase
  		user.save
  	end
  end
end
