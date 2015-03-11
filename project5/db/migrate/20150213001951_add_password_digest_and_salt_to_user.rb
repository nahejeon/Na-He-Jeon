class AddPasswordDigestAndSaltToUser < ActiveRecord::Migration
  def change
  	add_column :users, :password_digest, :string
  	add_column :users, :salt, :string
  	User.reset_column_information
  	User.find_each do |user|
      salt = Random.rand.to_s
      user.salt = salt
      password_digest = Digest::SHA1.hexdigest(user.login + salt)
  		user.password_digest = password_digest
  		user.save()
  	end
  end
end
